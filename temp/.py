import torch
from model import Model

model = Model()

torch.onnx.export(
    model
)