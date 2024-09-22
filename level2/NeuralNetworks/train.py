import torch
from myModel import MyModel
import torchvision


train_data = torchvision.datasets.MNIST(
    root='MNIST',
    train=True,
    transform=torchvision.transforms.ToTensor(),
    download=True
)

test_data = torchvision.datasets.MNIST(
    root='MNIST',
    train=False,
    transform=torchvision.transforms.ToTensor(),
    download=True
)

epoch = 5
batch_size = 512

model = MyModel().cuda()
optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
loss_func = torch.nn.CrossEntropyLoss()

train_loader = torch.utils.data.DataLoader(
    dataset=train_data,
    batch_size=batch_size,
    shuffle=True,
)

test_loader = torch.utils.data.DataLoader(
    dataset=test_data,
    batch_size=batch_size,
    shuffle=True,
)

for i in range(epoch):

    loss_list = []

    for data, label in train_loader:
        model.train()

        data = data.cuda()
        label = label.cuda()

        prediction = model(data)
        loss = loss_func(prediction, label.view(-1))

        loss_list.append(loss.cpu().item())

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

    print('train_loss is: {} in epoch {}'.format(sum(loss_list)/len(loss_list), i))

    loss_list = []

    for data, label in test_loader:
        model.eval()

        data = data.cuda()
        label = label.cuda()

        prediction = model(data)
        loss = loss_func(prediction, label.view(-1))

        loss_list.append(loss.cpu().item())

    print('valid_loss is: {} in epoch {}'.format(sum(loss_list) / len(loss_list), i))

torch.save(model, 'model.pth')
