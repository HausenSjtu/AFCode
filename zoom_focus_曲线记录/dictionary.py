import re

inputName = 'data_5.txt'
fin = file(inputName)
lines = fin.readlines()
fin.close()

dic={}

for line in lines:
    zf = re.findall(r'=\d*',line)
    z = zf[0].strip('=')
    f = zf[1].strip('=')
    try:
        f = int(f)
    except:
        print f
        print z
        f=-100
    z = int(z)
    dic[z] = f

zs = dic.keys()
zs.sort()
train = 50
emp=[]

##while fail != 0:
##    fail = 0
##    for i in range(0,4993,32):
##        try:
##            zs.index(i)
##        except:
##            print "can't find:%d" %i
##            j=0
##            fail_=True
##            while fail_==True:
##                fail_= False
##                try:
##                    dic[i] = (dic[i-32*j]+dic[i+32*j])/2
##                    zs.append(i)
##                except:
##                    fail += 1
##                    fail_=True
##                    j+=1
##                    print "no neigb fail:%d" %fail
##    print "=============================="

for i in range(0,4993,32):
    try:
        zs.index(i)
    except:
        emp.append(i)
        zs.append(i)
        dic[i]=dic[i-32]


for i in range(0,train):
    for em in emp:
        dic[em] = (dic[em-32]+dic[em+32])*0.5
        dic[em] = int(dic[em])

zs.sort()


for i in range(0,4993,32):
    try:
        zs.index(i)
    except:
        print i




fout = file(inputName[:len(inputName)-4]+'_out.txt','w')
for x in zs:
#    fout.writelines('zoom='+str(x)+'\tfocus='+dic[x]+'\n')
    fout.writelines(str(x)+' '+str(dic[x])+'\n')
fout.close()


fout = file(inputName[:len(inputName)-4]+'_out_2.txt','w')
for x in zs:
#    fout.writelines('zoom='+str(x)+'\tfocus='+dic[x]+'\n')
    fout.writelines(str(dic[x])+',')
fout.close()

print "complete~"
