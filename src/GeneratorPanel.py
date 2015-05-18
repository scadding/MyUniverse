import wx

class GeneratorPanel(wx.Panel):
	def __init__(self, parent, generator):
		self.generator = generator
		wx.Panel.__init__(self, parent, wx.ID_ANY)
		self.labels = dict()
		self.fields = dict()
		for n in self.generator.pList:
			self.labels[n] = wx.StaticText(self, -1, n)
			if type(self.generator.parameters[n]) is list:
				if self.generator.parameters[n]:
					self.fields[n] = wx.ComboBox(self, -1, choices=self.generator.parameters[n], style=wx.CB_DROPDOWN)
					self.Bind(wx.EVT_COMBOBOX, self.onUpdate, self.fields[n])
				else:
					self.fields[n] = wx.ListBox(self, -1)
			elif type(self.generator.parameters[n]) is dict:
				l = []
				for a in self.generator.parameters[n]:
					l.append(a)
				self.fields[n] = wx.ComboBox(self, -1, choices=l, style=wx.CB_DROPDOWN)
			else:
				self.fields[n] = wx.TextCtrl(self, -1, self.generator.parameters[n])
	def do_layout(self):
		panelSizer = wx.BoxSizer(wx.VERTICAL)
		for n in self.generator.pList:
			v = 0
			if type(self.fields[n]) is wx.ListBox:
				paramSizer = wx.BoxSizer(wx.VERTICAL)
				v = 10
			else:
				paramSizer = wx.BoxSizer(wx.HORIZONTAL)
			paramSizer.Add(self.labels[n], 0, wx.ALIGN_CENTER)
			paramSizer.Add(self.fields[n], 1, wx.EXPAND, 0)
			panelSizer.Add(paramSizer, v, wx.ALL | wx.EXPAND, 4)
		self.SetSizer(panelSizer)
	def onUpdate(self, e):
		p = dict()
		for n in self.generator.parameters:
			if type(self.fields[n]) is not wx.ListBox:
				if self.fields[n].GetValue() != "":
					p[n] = self.fields[n].GetValue()
		self.generator.Update(p)
		for n in self.generator.parameters:
			if type(self.fields[n]) is wx.ListBox:
				self.fields[n].SetSelection(-1)
				self.fields[n].Clear()
				self.fields[n].Set(self.generator.parameters[n])
	def Roll(self, numRolls):
		p = dict()
		for n in self.generator.parameters:
			if type(self.fields[n]) is wx.ListBox:
				value = self.fields[n].GetStrings()[self.fields[n].GetSelection()]
			else:
				value = self.fields[n].GetValue()
			if value != "":
				p[n] = value
		return self.generator.roll(p, numRolls)
