from Utils.trainer import Trainer
import argparse

parser = argparse.ArgumentParser()

parser.add_argument("--iterations", type=int, default=1000000, help="Iterations the model was trained")
parser.add_argument("--batch_size", type=int, default=32, help="Size of mini batch")
parser.add_argument("--conv_layers", type=int, default=8, help="Num of conv layers")
parser.add_argument("--epsilon", type=float, default=1, help="Epsilon in epsilon-greedy algorithm")
parser.add_argument("--lr", type=float, default=0.0001, help="Learning rate")
parser.add_argument("--copy_step", type=int, default=3000, help="Iterations between copy")
parser.add_argument("--epsilon_decay", type=float, default=0.9998, help="Epsilon-decay in epsilon-greedy algorithm")
parser.add_argument("--optim", type=str, default='adam', help="Optimizer of this model")
parser.add_argument("--capacity", type=int, default=10000, help="Capacity of exp queue")
parser.add_argument("--gamma", type=float, default=0.9, help="Weight of future reward")
parser.add_argument("--device", type=str, default='cuda', help="Device to use")
parser.add_argument("--print_step", type=int, default=2, help="Iterations between log")
parser.add_argument("--model_name", type=str, default='DQN_v3', help="Name of the model")

args = parser.parse_args()


if __name__ == '__main__':
    with open('../log.txt', 'w') as f:
        f.write('')

    trainer = Trainer(args)

    trainer.fit()



