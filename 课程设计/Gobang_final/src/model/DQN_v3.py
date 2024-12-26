import torch.nn as nn
import torch
from copy import deepcopy


class DQN(nn.Module):

    def __init__(self, conv_layers):
        super(DQN, self).__init__()

        self.conv_layers = nn.ModuleList()
        self.conv_layers.append(nn.Conv2d(1, 64, 5, 1, 2))
        self.conv_layers.append(nn.Conv2d(64, 256, 5, 1, 2))
        self.conv_layers.append(nn.Conv2d(256, 512, 5, 1, 2))

        for i in range(conv_layers):
            self.conv_layers.append(nn.Conv2d(512, 512, 3, 1, 1))

        self.res_layers = nn.ModuleList()
        self.res_layers.append(nn.Conv2d(1, 64, 1))
        self.res_layers.append(nn.Conv2d(64, 256, 1))
        self.res_layers.append(nn.Conv2d(256, 512, 1))

        for i in range(conv_layers):
            self.res_layers.append(nn.Conv2d(512, 512, 1))

        self.output = nn.Conv2d(512, 64, 1)

        self.mlp = nn.Linear(64 * 15 * 15, 512)

        self.v = nn.Linear(512, 1)
        self.a = nn.Linear(512, 15 * 15)

    def forward(self, data):

        data = deepcopy(data)
        for i in range(len(self.conv_layers)):
            data = self.conv_layers[i](data) + self.res_layers[i](data)
            data = nn.ReLU()(data)

        data = self.output(data)
        data = data.view(-1, 64 * 15 * 15)
        data = self.mlp(data)

        a = self.a(data)
        v = self.v(data)

        a = a - torch.mean(a, dim=-1).unsqueeze(-1)

        return a + v
