import sys

RPM = raw_input("Desired RPM (<=591): ")
RPM = int(RPM)
if RPM > 591 or RPM < 2:
  print ("BAD INPUT")
  sys.exit(0)

MAX_RPM = 591

step = 0
dmax = float(MAX_RPM)
if RPM<=MAX_RPM/2.0:
  step = 1
  dmax = MAX_RPM/2.0
if RPM<=MAX_RPM/4.0:
  step = 2
  dmax = MAX_RPM/4.0
if RPM<=MAX_RPM/8.0:
  step = 3
  dmax = MAX_RPM/8.0
if RPM<=MAX_RPM/16.0:
  step = 7
  dmax = MAX_RPM/16.0

count = int((dmax/RPM)*(32.0+1.0)-1)

#print (step,count)
actual_rpm = int(dmax*33/(count+1))
print "requested rpm:{}, actual: {}".format(RPM,actual_rpm)

 
sum = 16+255*14+step+count
cksum = (-sum)&255
s = ":10000000{:02X}{:02X}FFFFFFFFFFFFFFFFFFFFFFFFFFFF{:02X}".format(step,count,cksum)
#print(s)
#end code
#print(":00000001FF")
with open("{:d}rpm.eep".format(actual_rpm),'wb') as outf:
  outf.write(s+"\n")
  outf.write(":00000001FF")

raw_input("done! rom file written to {:d}rpm.eep. Press any key to exit...".format(actual_rpm))