# README #


### requirements.txt: ###

aioconsole==0.4.1
aioprocessing==2.0.1
asyncio==3.4.3
bleak==0.20.1
pycryptodome==3.7.3
transitions==0.8.11

### Installation ###
* Install python3
    https://www.python.org/downloads/


* pip3 install -r requirements.txt


### Virtual Environment ###
In some cases where there is a conflict or difficulty with the environment you may wish to use a virtual environment.

Install venv as appropriate for your platform:
https://packaging.python.org/en/latest/guides/installing-using-pip-and-virtual-environments/

python3 -m venv venv
cd venv/
source bin/activate
pip3 install asyncio
pip3 install aioconsole
pip3 install bleak
pip3 install transitions
pip3 install pycryptodome
pip3 install Cipher
pip3 install AES
pip3 install chacha20poly1305
git clone ...
cd ...

### Testing ###

 ./src/Generators/tablegen/table.py --data=/home/doug/Development/mu/MyUniverse/Data --table=test
 ls -ltr test/
 ./src/Generators/tablegen/table.py --data=/home/doug/Development/mu/MyUniverse/Data --run=Test
./src/Generators/tablegen/table.py --test --data=/home/doug/Development/mu/MyUniverse/Data --groups

 ./src/Generators/tablegen/table.py --data=/home/doug/Development/mu/MyUniverse/Data --run=csv
 
 
./src/Generators/tablegen/table.py --help
Usage: table.py [options]

Options:
  -h, --help            show this help message and exit
  -d Data, --data=Data  Data Directory
  -a, --all             
  --test                
  -i, --import          
  -l, --listen          
  -s, --server          
  -t Table, --table=Table
                        Table Name
  -g Group, --group=Group
                        Group Name
  -r RUN, --run=RUN     
  --groups              
  --tables=TABLES       
  
./src/Generators/tablegen/table.py --data=/home/doug/Development/mu/MyUniverse/Data --group=csv

Organization
Traveller
Occupation
Food
Rolemaster
csv
Places
Spy
Oracles
Magic
Names
SGNP
A
Primetime
Monster
Utility
Weapons
Religion
People
Unknown
Clothes
Fantasy
Animals
Asia
Plot
Generators
Dragons
SciFi
Pulp


./src/Generators/tablegen/table.py --data=/home/doug/Development/mu/MyUniverse/Data --tables=csv




