This is the respository which host the source code, 3D models and schematics for Socavabot combat robot.

![](Graphics/cover1.jpg)
![](Graphics/cover2.jpg)

## Parts
| Name               |      Dimensions     | Weight | Image         |
| ------------------ |:-------------------:|:------:| -------------:|
| ESP-12             |  24x16 mm           |        | <img src="3DModels/ESP-12/image.jpg" alt="drawing" width="200"/> |
| Generic Wheel      |                     |        | <img src="3DModels/Wheel/image.jpg" alt="drawing" width="200"/> |
| Wheel Hub          |                     |        | <img src="3DModels/WheelHub/image.jpg" alt="drawing" width="200"/> |
| Pololu Micro Motor |   35x12x10 mm       |        | <img src="3DModels/PololuMicroMotor/image.jpg" alt="drawing" width="200"/> |
| L298N H-Bridge     |    50x52 mm         |        | <img src="3DModels/L298N/image.jpg" alt="drawing" width="200"/> |
| Li-Po Battery      |  60x30x15 mm        |        | <img src="3DModels/Battery/image.jpg" alt="drawing" width="200"/> |
| Brushless Motor    |                     |        | <img src="3DModels/BrushlessMotor/image.jpg" alt="drawing" width="200"/> |
| 3D printed chassis | D: 170 mm, H: 60 mm |        | <img src="3DModels/Chassis/image.png" alt="drawing" width="200"/> |
| Metal Sheets       |                     |        |               |

## 3D Model Design

For the `chassis` and the `wheel hub` were applied topology optimization techniques in order to reduce mass:

### Wheel Hub

Original Design:  
<img src="3DModels/WheelHub/original.jpg" alt="drawing" width="300"/>

Stress Analysis:  
<img src="3DModels/WheelHub/stress.jpg" alt="drawing" width="500"/>

Optimized Design (Mass reduction 80%):  
<img src="3DModels/WheelHub/current.jpg" alt="drawing" width="400"/>

### Assemble

![](Graphics/HubWheelAssemble.gif)

## Schematics
The connections between the motors, ESP-12 and H-bridge:  

<img src="Graphics/Schematics/connections.jpg" alt="drawing"/>