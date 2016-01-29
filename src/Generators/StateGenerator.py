
from tablegen import table

class StateGenerator:
    def __init__(self):
        self.tm = table.tableMgr()
        table.walktree('Data/Tables', self.tm.addfile)
        self.parameters = dict()
        self.parameters['Seed'] = ['', '0']
        self.parameters['Group'] = self.GetGeneratorGroups()
        self.parameters['Generators'] = []
        self.pList = ['Seed', 'Group', 'Generators']
    def GetGeneratorGroups(self):
        # Get list of generators
        groupList = []
        for t in self.tm.groups():
            groupList.append(t)
        groupList.sort()
        return groupList
    def Update(self, p):
        self.parameters['Generators'] = self.GetGeneratorList(p['Group'])
        pass
    def GetGeneratorList(self, p):
        # Get list of generators
        genList = []
        for x in self.tm.group[p]:
            genList.append(x)
        genList.sort()
        return genList
    def roll(self, p, numRolls):
        t = u''
        if p.has_key('Seed'):
            self.tm.setSeed(int(p['Seed']))
        if p.has_key('Generators'):
            t = p['Generators']
        results = u''
        for j in range(numRolls):
            results += self.tm.roll(t)
            results += '<br>'
        return t, results
        
