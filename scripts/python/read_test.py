# test_read.py
from read_write_model import read_model

path = "/home/clink/Data/SFM/person-hall/models/0"  # sparse/0 文件夹的路径
cameras, images, points3D = read_model(path, ext=".bin")

print(f"{len(cameras)} cameras")
print(f"{len(images)} images")
print(f"{len(points3D)} points")

