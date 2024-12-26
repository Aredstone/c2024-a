from copy import deepcopy
def in_chessboard(x, y):
    return 0 <= x < 15 and 0 <= y < 15


eps = 0.01


def eq(x, y):
    return abs(x - y) < eps


dx = [1, 1, 1, 0]
dy = [0, 1, -1, 1]

score_list = [[
    ('xxxxx', 1.2),
    ('.xxxx.', 0.85),
    ('xxx.x', 0.7),
    ('x.xxx', 0.7),
    ('xx.xx', 0.7),
    ('.xxxx', 0.5),
    ('xxxx.', 0.5),
    ('.xxx.', 0.3),
    ('.x.xx.', 0.25),
    ('.xx.x.', 0.25),
    ('xx.x.', 0.2),
    ('x.xx.', 0.2),
    ('.xx.x', 0.2),
    ('.x.xx', 0.2),
    ('.xxx', 0.1),
    ('xxx.', 0.1)
], [
    ('.xxxx.', 1.2),
    ('xxx.x', 1),
    ('x.xxx', 1),
    ('xx.xx', 1),
    ('.xxxx', 0.8),
    ('xxxx.', 0.8),
    ('.xxx.', 0.6),
    ('.x.xx.', 0.4),
    ('.xx.x.', 0.4),
    ('xx.x.', 0.3),
    ('x.xx.', 0.3),
    ('.xx.x', 0.3),
    ('.x.xx', 0.3),
    ('.xxx', 0.1),
    ('xxx.', 0.1)
],
]


def calculate(save, flag):
    tmp_value = 0
    done = 0
    f = 0
    for tmp in save:
        if score_list[0][0][0] in tmp:
            done = 1
        for (s, v) in score_list[flag]:
            if s in tmp:
                f = 1
                tmp_value += v
                break
    if tmp_value == 0 and not f and not flag:
        return -0.05, done
    return tmp_value, done


def get_all_direct(state, x_, y_, now, vis=None):
    save = []
    for i in range(4):
        if i == 1:
            pass
        tmp = ''
        xx = deepcopy(x_)
        yy = deepcopy(y_)
        zero = 0
        while (in_chessboard(xx + dx[i], yy + dy[i]) and
               (eq(state[xx + dx[i]][yy + dy[i]], now) or eq(state[xx + dx[i]][yy + dy[i]], 0))):
            xx = xx + dx[i]
            yy = yy + dy[i]
            if eq(state[xx][yy], 0):
                if zero:
                    break
                zero = 1
        if eq(state[xx][yy], now):
            tmp += 'x'
        elif eq(state[xx][yy], 0):
            if len(tmp) == 0 or tmp[-1] != '.':
                tmp += '.'
        else:
            tmp += 'o'
        zero = 0
        if vis is not None:
            vis[xx][yy] = 1
        while (in_chessboard(xx - dx[i], yy - dy[i]) and
               (eq(state[xx - dx[i]][yy - dy[i]], now) or eq(state[xx - dx[i]][yy - dy[i]], 0))):
            xx = xx - dx[i]
            yy = yy - dy[i]
            if vis is not None:
                vis[xx][yy] = 1
            if eq(state[xx][yy], now):
                tmp += 'x'
            elif eq(state[xx][yy], 0):
                if len(tmp) == 0 or tmp[-1] != '.':
                    tmp += '.'
            else:
                tmp += 'o'
            if eq(state[xx][yy], 0) and len(tmp) != 0:
                if len(tmp) == 1:
                    if tmp[0] == '.':
                        continue
                if zero:
                    break
                zero = 1

        save.append(tmp)

    return save, vis
