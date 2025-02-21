# README #

### Requirements ###
sudo apt install python3.12-venv
sudo apt install python3-pip
sudo apt install python3-wx*

sudo apt install python3-imp
sudo apt install python3-sha
sudo apt install python3-hashlib
sudo apt install python3-xmlrpclib


### requirements.txt: ###

wxPython>=4.0

xmlrpclib
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
source venv/bin/activate
pip3 install -r requirements.txt
cd MyUniverse

deactivate


export PYTHONPATH=$PYTHONPATH:$PWD
cd ...

### Testing ###

 ./src/Generators/tablegen/table.py --data=$PWD/Data --table=test
 ls -ltr test/
 ./src/Generators/tablegen/table.py --data=$PWD/Data --run=Test
./src/Generators/tablegen/table.py --test --data=$PWD/Data --groups

 ./src/Generators/tablegen/table.py --data=$PWD/Data --run=csv
 
 
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
  
./src/Generators/tablegen/table.py --data=$PWD/Data --group=csv

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


./src/Generators/tablegen/table.py --data=$PWD/Data --tables=csv




