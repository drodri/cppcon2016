import shutil, os, sys

if not os.path.exists("build"):
  os.mkdir("build")
os.chdir("build")

if not os.path.exists("conanbuildinfo.cmake"):
  os.system("conan install ../pythoncpp -s arch=x86 --build=missing")
  os.system('cmake ../pythoncpp -G "Visual Studio 14"')

os.system('cmake --build . --config Release')
os.chdir("bin")


def my_log(msg):
  print "\n MY MSG! ", msg


def run_module(m):
  m.set_log_function(my_log)
  print "\n*** RUNNING ", m.__name__, " ******"
  print "Addition:", m.add(2, 3)
  print "Multiply:", m.multiply(3, 5)

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
  print "Happy ", duffy.happiness, " food ", food.quantity
  water = m.Water()
  water.amount = 2.3
  duffy.give(water)
  print "Happy ", duffy.happiness

  silvester = m.Looney("Silvester")
  print "Average happines ", m.average([duffy, silvester])

  silvester.friends = ["Taz", "Bugs"]
  duffy.friends = ["Elmer"]
  print "Silvester friends ", silvester.friends
  print "All friends ", m.collect([duffy, silvester])



def run():
  sys.path.append(os.getcwd())
  import boost_math
  import pybind11_math

  run_module(boost_math)
  run_module(pybind11_math)


run()

