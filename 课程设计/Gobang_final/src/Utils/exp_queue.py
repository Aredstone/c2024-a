import random
import torch


class ExpQueue:

    def __init__(self, capacity, batch_size, device):

        self.queue = []
        self.win = []
        self.capacity = capacity
        self.batch_size = batch_size
        self.device = device
        self.pos = 0

    def push(self, state, action, reward, next_state, is_done):
        exp_package = (state, action, reward, next_state, is_done)

        if len(self.queue) < self.capacity:
            self.queue.append(exp_package)

        else:
            self.queue[self.pos] = exp_package
            self.pos = (self.pos + 1) % self.capacity

    def sample(self):

        data = random.sample(self.queue, self.batch_size)

        state, action, reward, next_state, is_done = zip(*data)
        state = torch.stack(state).to(self.device).unsqueeze(1)

        tmp_action = []
        for act in action:
            tmp_action.append(act[0] * 15 + act[1])

        action = torch.tensor(tmp_action, device=self.device).view(-1, 1)
        reward = torch.tensor(reward, device=self.device)
        next_state = torch.stack(next_state).to(self.device).unsqueeze(1)
        is_done = torch.tensor(is_done, device=self.device)

        return state, action, reward, next_state, is_done


