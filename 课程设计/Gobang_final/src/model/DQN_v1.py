import torch.nn as nn
import torch
from copy import deepcopy


class DQN(nn.Module):

    def __init__(self, conv_layers):
        super(DQN, self).__init__()

        self.input_layers = []
        self.input_layers.append(nn.Conv2d(1, 128, 3, padding=1))
        self.input_layers.append(nn.Conv2d(128, 512, 3, padding=1))
        self.input_layers.append(nn.Conv2d(512, 2048, 3, padding=1))

        self.res_convs = []
        self.res_convs.append(nn.Conv2d(1, 128, 1, padding=0))
        self.res_convs.append(nn.Conv2d(128, 512, 1, padding=0))
        self.res_convs.append(nn.Conv2d(512, 2048, 1, padding=0))

        self.input_layers = nn.ModuleList(self.input_layers)
        self.res_convs = nn.ModuleList(self.res_convs)
        self.conv_layers = []
        for i in range(conv_layers):
            self.input_layers.append(nn.Conv2d(2048, 2048, 3, padding=1))
            self.input_layers.append(nn.ReLU())

        self.conv_layers = nn.ModuleList(self.conv_layers)

        self.GAP = nn.AdaptiveAvgPool2d(1)

        self.output_layers = []
        self.output_layers.append(nn.Linear(2048, 512))
        self.output_layers.append(nn.ReLU())
        self.output_layers.append(nn.Linear(512, 15 * 15))
        self.output_layers.append(nn.Linear(15 * 15, 15 * 15))
        self.output_layers = nn.Sequential(*self.output_layers)

    def forward(self, data):
        # print(data)
        x = deepcopy(data)
        for i in range(3):
            x = nn.ReLU()(self.input_layers[i](x) + self.res_convs[i](x))
        for layer in self.conv_layers:
            x = layer(x) + x

        x = self.GAP(x).squeeze()
        x = self.output_layers(x)
        return x
