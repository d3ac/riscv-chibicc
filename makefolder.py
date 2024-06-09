import os

for i in range(32):
    if not os.path.exists('lesson/{}-{}'.format(i*10+1, i*10+10)):
        os.makedirs('lesson/{}-{}'.format(i*10+1, i*10+10))
    for j in range(10):
        if not os.path.exists('lesson/{}-{}/{}'.format(i*10+1, i*10+10, j+1+10*i)):
            os.makedirs('lesson/{}-{}/{}'.format(i*10+1, i*10+10, j+1+10*i))