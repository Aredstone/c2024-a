from exp_queue import ExpQueue
from reward_func_v2 import reward_func
from src.model.DQN_v3 import DQN

import os
import torch
import random
from copy import deepcopy


class Trainer:

    def __init__(self, args):
        self.iterations = args.iterations
        self.batch_size = args.batch_size
        self.conv_layers = args.conv_layers
        self.epsilon = args.epsilon
        self.lr = args.lr
        self.copy_step = args.copy_step
        self.epsilon_decay = args.epsilon_decay
        self.optim = args.optim
        self.capacity = args.capacity
        self.gamma = args.gamma
        self.loss_func = torch.nn.MSELoss()
        self.device = args.device
        self.print_step = args.print_step
        self.model_name = args.model_name
        self.load_from = args.load_from

        if self.load_from is None:
            self.model = DQN(conv_layers=args.conv_layers).to(self.device)
            self.target = DQN(conv_layers=args.conv_layers).to(self.device)
        else:
            self.model = torch.load('../trained_model/' + self.model_name + '/model' + str(self.load_from) + '.pth')
            self.target = torch.load('../trained_model/' + self.model_name + '/model' + str(self.load_from) + '.pth')

        self.queue = ExpQueue(self.capacity, self.batch_size, self.device)

        if self.optim == 'adam':
            self.optim = torch.optim.Adam(self.model.parameters(), lr=self.lr)
        elif self.optim == 'sgd':
            self.optim = torch.optim.SGD(self.model.parameters(), lr=self.lr)

        self.null_state = torch.zeros((15, 15)).to(self.device)

        if not os.path.isdir('../trained_model/' + self.model_name):
            os.makedirs('../trained_model/' + self.model_name)

    def show(self, _state_, action):
        print('state: ')
        flag = 1
        for i in range(15):
            for j in range(15):
                if i == action[0].detach().cpu().item() and j == action[1].detach().cpu().item():
                    if abs(_state_[i][j]) > 0.1:
                        flag = 0
                    print('@', end=' ')
                elif abs(_state_[i][j]) < 0.1:
                    print('.', end=' ')
                elif abs(_state_[i][j] - 1) < 0.1:
                    print('X', end=' ')
                else:
                    print('O', end=' ')
            print()
        if not flag:
            print("err choice")

    def fit(self):

        now_state = deepcopy(self.null_state)
        loss_list = []

        for it in range(self.iterations):

            if it % self.copy_step == 0:
                self.target.load_state_dict(self.model.state_dict())

            # ======== data collecting stage ======== #

            self.model.eval()
            if it % self.print_step == 0:
                predict = self.model(now_state.unsqueeze(0).unsqueeze(0)).squeeze().view(-1)
                _, tmp_id = torch.max(predict, dim=0)
                print(_, tmp_id)

                action = (tmp_id // 15, tmp_id % 15)

                print(f'it: {it}')
                self.show(now_state, action)

            tmp_now_state = deepcopy(now_state)

            if random.random() < self.epsilon:
                tmp_action = (random.randint(0, 14), random.randint(0, 14))
            else:
                predict = self.model(now_state.unsqueeze(0).unsqueeze(0)).squeeze()
                _, tmp_id = torch.max(predict, dim=0)

                tmp_action = (tmp_id // 15, tmp_id % 15)
            if self.epsilon > 0.1:
                self.epsilon *= self.epsilon_decay

            tmp_reward, tmp_next_state, tmp_is_done = reward_func(now_state, tmp_action)
            self.queue.push(tmp_now_state, tmp_action, tmp_reward, tmp_next_state, tmp_is_done)

            if tmp_is_done:
                now_state = deepcopy(self.null_state)
            else:
                now_state = deepcopy(tmp_next_state)

            if it < self.batch_size:
                continue

            # =========== training stage =========== #

            state, action, reward, next_state, is_done = self.queue.sample()
            self.model.train()

            for i in range(5):
                predict = self.model(state).gather(1, action).squeeze().view(-1)
                feature_label = torch.max(self.target(next_state), dim=-1).values.view(-1)
                label = reward + (feature_label * (1 - is_done) * self.gamma * -1)
                loss = self.loss_func(predict, label)

                loss_list.append(loss.detach().cpu().item())

                self.optim.zero_grad()
                loss.backward()
                self.optim.step()

            if it % self.print_step == 0:
                print(f'it: {it}, loss: {sum(loss_list) / len(loss_list)}')
                with open('../log.txt', 'a') as f:
                    f.write(f'it: {it}, loss: {sum(loss_list) / len(loss_list)}\n')
                if it % 20000 == 0:
                    torch.save(self.model, '../trained_model/' + self.model_name + '/model' + str(it + self.load_from) + '.pth')
                loss_list = []

