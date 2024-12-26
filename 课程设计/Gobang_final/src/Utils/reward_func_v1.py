from copy import deepcopy


def in_checkerboard(i, j):
    return 0 <= i < 15 and 0 <= j < 15


def calculate(num):
    if num[0] != 0 and num[2] != 0:
        return 0
    elif num[0] == 1 and num[1] == 4:
        return 0.1
    elif num[0] == 2 and num[1] == 3:
        return 0.5
    elif num[0] == 3 and num[1] == 2:
        return 1
    elif num[0] == 4 and num[1] == 1:
        return 5
    elif num[0] == 5:
        return 80
    elif num[2] == 1 and num[1] == 4:
        return -0.1
    elif num[2] == 2 and num[1] == 3:
        return -0.5
    elif num[2] == 3 and num[1] == 2:
        return -1
    elif num[2] == 4 and num[1] == 1:
        return -5
    elif num[2] == 5:
        return -80
    else:
        return 0


dx = [[0, 1, 2, 3, 4], [0, 0, 0, 0, 0], [0, 1, 2, 3, 4], [0, -1, -2, -3, -4]]
dy = [[0, 1, 2, 3, 4], [0, 1, 2, 3, 4], [0, 0, 0, 0, 0], [0, 1, 2, 3, 4]]


def get_num(state, a, b):
    num_save = []

    for i in range(4):
        flag = 1
        num = [0, 0, 0]
        for j in range(5):
            x = dx[i][j] + a
            y = dy[i][j] + b

            if not in_checkerboard(x, y):
                flag = 0
                break

            if state[x][y] > 0.1:
                num[0] += 1
            elif state[x][y] < -0.1:
                num[2] += 1
            elif abs(state[x][y]) < 0.1:
                num[1] += 1

        if flag:
            num_save.append(num)

    return num_save


def value(state):
    val = 0
    for i in range(15):
        for j in range(15):
            nums = get_num(state, i, j)
            for num in nums:
                val += calculate(num)
    return val


def apply_action(state_, action):
    state = deepcopy(state_)
    if abs(state_[action[0]][action[1]]) > 0.1:
        return state, 0
    state[action[0]][action[1]] = 1
    return state, 1


def check_done(state):
    for i in range(15):
        for j in range(15):
            nums = get_num(state, i, j)
            for num in nums:
                if num[0] == 5 or num[2] == 5:
                    return 1
    return 0


# return reward & is done
def reward_func(state_, action):
    state = deepcopy(state_)
    state = state.squeeze()
    if len(state.shape) > 2 or state.shape[0] != 15 or state.shape[1] != 15:
        raise RuntimeError("reward_func receive wrong state.")

    reward1 = value(state)
    state, flag = apply_action(state, action)
    if not flag:
        # print('err!')
        return -80, state, 1
    is_done = check_done(state)
    reward2 = value(state)

    return reward2 - reward1, state * -1, is_done
