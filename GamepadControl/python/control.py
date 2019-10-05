import pygame
import socket

UDP_IP = '192.168.4.1'
UDP_PORT = 42069

pygame.init()

joysticks = [pygame.joystick.Joystick(i) for i in range(pygame.joystick.get_count())]
for joy in joysticks:
  joy.init()

def send_to_robot(command):
  print(command)
  sock = socket.socket(socket.AF_INET, # Internet
                       socket.SOCK_DGRAM) # UDP
  sock.sendto(command.encode('ascii'), (UDP_IP, UDP_PORT))

while True:
    for event in pygame.event.get():
      if event.type == pygame.JOYAXISMOTION:
        joystick_values = event.__dict__
        axis = joystick_values['axis'] # Possible values: 0 1 2 3
        value = int(joystick_values['value']) # Possible values: 0.1 0.5 1.0 0.9 0.0

        # UDP packet content: AxisValue
        # e.g. '00.8'  , '10.4'  , '2-1.0'  ,  etc.
        command = '{}{:+}'.format(axis, value)

        send_to_robot(command)
      elif event.type == pygame.JOYBUTTONDOWN:
        print(event)