import torch.nn as nn
import torch
from copy import deepcopy


class DQN(nn.Module):

    def __init__(self, conv_layers):
        super(DQN, self).__init__()

        self.input_layers = []
        self.input_layers.append(nn.Conv2d(1, 8, 5, padding=0))
        self.input_layers.append(nn.Conv2d(8, 32, 5, padding=0))
        self.input_layers = nn.Sequential(*self.input_layers)

        self.conv_layers = []
        for i in range(conv_layers):
            self.input_layers.append(nn.Conv2d(32, 32, 3, padding=1))
            self.input_layers.append(nn.ReLU())

        self.conv_layers = nn.ModuleList(self.conv_layers)

        self.output_layers = []

        self.output_layers.append(nn.Linear(32 * 7 * 7, 256))
        self.input_layers.append(nn.ReLU())
        self.output_layers.append(nn.Linear(256, 256))
        self.input_layers.append(nn.ReLU())
        self.output_layers.append(nn.Linear(256, 256))
        self.input_layers.append(nn.ReLU())

        self.output_layers.append(nn.Linear(256, 15 * 15))

        self.output_layers.append(nn.Linear(15 * 15, 15 * 15))

        self.output_layers = nn.Sequential(*self.output_layers)

    def forward(self, data):

        x = deepcopy(data)
        y = deepcopy(data)

        x = nn.ReLU()(self.input_layers(x))
        for layer in self.conv_layers:
            x = layer(x) + x

        x = x.view(len(x), -1)
        x = self.output_layers(x)

        y = y.squeeze(0)
        y = y.view(-1, 15 * 15)
        y = torch.abs(y) * -1.0

        return x + y





