import torch.nn as nn
import torch


class MyModel(nn.Module):

    def __init__(self):
        super(MyModel, self).__init__()

        self.input_layer = nn.Linear(
            in_features=28 * 28,
            out_features=512
        )

        self.hidden_layer1 = nn.Linear(
            in_features=512,
            out_features=1024
        )

        self.hidden_layer2 = nn.Linear(
            in_features=1024,
            out_features=256
        )

        self.output_layer = nn.Linear(
            in_features=256,
            out_features=10
        )

    def forward(self, data):
        data = data.view(-1, 28 * 28)
        data = self.input_layer(data)

        data = self.hidden_layer1(data)
        data = torch.relu(data)

        data = self.hidden_layer2(data)
        data = torch.relu(data)

        return self.output_layer(data)



