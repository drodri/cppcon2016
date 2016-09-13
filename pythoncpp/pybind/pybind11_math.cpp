#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <numeric>


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

std::set<std::string> collect(const std::vector<Pet>& v){
    std::set<std::string> result;
    for(Pet p: v){
        result.insert(std::begin(p.friends), std::end(p.friends));
    }
    
    return result;
}

namespace py = pybind11;

PYBIND11_PLUGIN(pybind11_math) {
    py::module m("pybind11_math");
    m.def("add", &add);

    m.def("average", &average);
    m.def("collect", &collect);

    py::class_<Food>(m, "Food")
        .def(py::init<>())
        .def_readwrite("quantity", &Food::quantity);

    py::class_<Water>(m, "Water")
        .def(py::init<>())
        .def_readwrite("amount", &Water::amount);

    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>(),
             py::arg("name") = std::string("Silvester"))
        .def("setName", &Pet::setName,
             py::arg("name") = std::string("Tweety"))
        .def("getName", &Pet::getName)
        .def_readwrite("happiness", &Pet::happiness)
        .def_readwrite("friends", &Pet::friends)
        .def("give", (void (Pet::*)(const Food&)) &Pet::give)
        .def("give", (void (Pet::*)(const Water&)) &Pet::give);

    return m.ptr();
}