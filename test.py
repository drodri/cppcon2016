import shutil, os, sys

if not os.path.exists("build"):
  os.mkdir("build")
os.chdir("build")

if not os.path.exists("conanbuildinfo.cmake"):
  os.system("conan install ../pythoncpp -s arch=x86")
  os.system('cmake ../pythoncpp -G "Visual Studio 14"')

os.system('cmake --build . --config Release')
os.chdir("bin")

def run_module(m):
  print "\n*** RUNNING ", m.__name__, " ******"
  print "Addition:", m.add(2, 3)

  print "Default name ", m.Looney().getName()
  pet = m.Looney()
  pet.setName()
  print "Default name ", pet.getName()

  duffy = m.Looney("duffy")
  print "Looney name ", duffy.getName()
  duffy.setName("Duffy")
  print "Looney name ", duffy.getName()
  food = m.Food()
  food.quantity = 1.2
  duffy.give(food)
  print "Happy ", duffy.happiness
  water = m.Water()
  water.amount = 2.3
  duffy.give(water)
  print "Happy ", duffy.happiness

  silvester = m.Looney("Silvester")
  print "Average happines ", m.average([duffy, silvester])

  silvester.friends = ["Taz", "Bugs"]
  duffy.friends = ["Elmer"]
  print "All friends ", m.collect([duffy, silvester])



def run():
  sys.path.append(os.getcwd())
  import boost_math
  import pybind11_math
  run_module(pybind11_math)
  run_module(boost_math)


run()

