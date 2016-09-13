#include <boost/python.hpp>
#include <string>
#include <numeric>
#include <vector>

int add(int i, int j) {
    return i + j;
}

struct Food {
    float quantity;
};
struct Water {
    float amount;
};
struct Pet {
    Pet(const std::string &name="Silvester") : name(name), happiness(0.0f) { }
    void setName(const std::string &name_="Tweety") { name = name_; }
    const std::string &getName() const { return name; }

    void give(const Food& food) { happiness += food.quantity;}
    void give(const Water& water) { happiness += water.amount;}

    std::string name;
    float happiness;
    std::vector<std::string> friends;
};

float average(const std::vector<Pet>& v){
    return std::accumulate(std::begin(v), std::end(v), 0.0f,
                          [](float a, Pet b) { return a + b.happiness;}) / v.size();
}

namespace py = boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(setname_overloads, Pet::setName, 0, 1)

BOOST_PYTHON_MODULE(boost_math){
    py::def("add", add);
    py::def("average", average);

    py::class_<Food>("Food")
        .def_readwrite("quantity", &Food::quantity);
    py::class_<Water>("Water")
        .def_readwrite("amount", &Water::amount);
    
    py::class_<Pet>("Pet", py::init<py::optional<std::string>>())
        .def_readwrite("happiness", &Pet::happiness)
        .def_readwrite("friends", &Pet::friends)
        .def("setName", &Pet::setName,
                setname_overloads())
        .def("getName", &Pet::getName,
                py::return_value_policy<py::copy_const_reference>())
        .def("give", (void (Pet::*)(const Food &)) &Pet::give)
        .def("give", (void (Pet::*)(const Water &)) &Pet::give);;
}