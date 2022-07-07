from serial import Serial

# pretty close but the value for 2+ digits isn't working


def main():
  s = Serial('/dev/tty.usbmodem141303', 38400, timeout=3)
  print(str(s.isOpen()))

  writeStr(s, 'led:1:state:1')
  print(readStr(s))
 
  writeStr(s, 'pwm:1:duty:35')
  print(readStr(s))

  s.close()



def readStr(s: Serial):
  return s.readline().decode('ascii')

def writeStr(s:Serial, data: str):
  data = data + '\n'
  s.write(data.encode('ascii'))

if __name__ == "__main__":
  main()