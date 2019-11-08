import pygame
import socket
import math

UDP_IP = '192.168.4.1'
UDP_PORT = 42069

BOOST_BUTTONS = [4, 5, 6, 7] #R1,L1,Start,Select
TUNELADORA_ON = [0] #A
TUNELADORA_OFF = [1] #B

pygame.init()
direction = gas = value = boost = tuneladora = 0
running = True

joysticks = [pygame.joystick.Joystick(i) for i in range(pygame.joystick.get_count())]
for joy in joysticks:
  joy.init()

def send_to_robot(command):
  byts = list(map(lambda n: n.to_bytes(1, byteorder='big', signed=True), command))
  print(byts)
  sock = socket.socket(socket.AF_INET, # Internet
                       socket.SOCK_DGRAM) # UDP
  sock.sendto(b''.join(byts), (UDP_IP, UDP_PORT))

while running:
  for event in pygame.event.get():
    event_values = event.__dict__
    #print(event)

    if event.type == pygame.QUIT:
      running = False
      break
    elif event.type == pygame.JOYAXISMOTION:
      axis = event_values['axis'] # Value range: [0-3] (int)
      value = int(event_values['value']) # Value range: [0-1] (float)
      if axis == 1:
        gas = -value
 	 
      elif axis == 3:
        direction = value
    elif event.type == pygame.JOYBUTTONDOWN:
      button = event_values['button']
      if button in BOOST_BUTTONS:
        boost = 1
      elif button in TUNELADORA_ON:
        tuneladora = 1
      elif button in TUNELADORA_OFF:
        tuneladora = 0
    elif event.type == pygame.JOYBUTTONUP:
      button = event_values['button']
      if button in BOOST_BUTTONS:
        boost = 0


    # UDP packet content: [+-Gas][+-Direction][Boost]
    command = [gas, direction, boost, tuneladora]

    send_to_robot(command)
