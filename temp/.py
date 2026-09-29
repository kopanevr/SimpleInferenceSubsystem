import torch
from model import Model

model = Model(input_dim=4, hidden_dim=16, output_dim=2)

model.eval()

print(model)

x = torch.rand(1, 4)

torch.onnx.export(
    model,
    x,
    "../models/model.onnx",
    input_names=["input"],
    output_names=["output"]
)