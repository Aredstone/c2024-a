import numpy as np
from PIL import Image, ImageOps
import tkinter as tk
import torch
from torchvision import transforms

# 创建一个全局变量用于保存点阵
matrix = np.zeros((28, 28), dtype=np.uint8)
transform = transforms.Compose([
    transforms.ToTensor()
])


# 点击时绘制点
def draw(event):
    x, y = event.x // 10, event.y // 10
    if x < 28 and y < 28:
        canvas.create_rectangle(x*10, y*10, (x+1)*10, (y+1)*10, fill="white")
        matrix[y, x] = 255

# 清除画布
def clear_canvas():
    global matrix
    canvas.delete("all")
    matrix = np.zeros((28, 28), dtype=np.uint8)

# 保存图片
def save_image():
    img = Image.fromarray(matrix)

    # 可以在这里加载你的MNIST模型并进行预测
    model = torch.load("model.pth")
    print(transform(img))
    prediction = model(transform(img).cuda())
    _, tmp = torch.max(prediction, 1)
    print(tmp.item())

# 创建Tkinter窗口
root = tk.Tk()
root.title("28x28 Dot Matrix")

canvas = tk.Canvas(root, width=280, height=280, bg="black")
canvas.pack()

# 绑定鼠标事件
canvas.bind("<B1-Motion>", draw)

# 添加按钮
button_clear = tk.Button(root, text="Clear", command=clear_canvas)
button_clear.pack()

button_save = tk.Button(root, text="Save", command=save_image)
button_save.pack()

root.mainloop()
