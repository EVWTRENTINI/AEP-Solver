#pragma once
#include <string>
#include <vector>
#include <memory>

class BeamSection {
public:
std::string name;
float area;
float inertia;

BeamSection() : name(""), area(0), inertia(0) {}

BeamSection(const char* n, float A, float I) : name(n), area(A), inertia(I) {}

template<class Archive>
void serialize(Archive& ar) {
    ar(cereal::make_nvp("name", name),
       cereal::make_nvp("area", area),
       cereal::make_nvp("inertia", inertia)
       );
}
};

class SectionManager {
public:
std::vector<std::shared_ptr<BeamSection>> sections;

SectionManager() {
    sections.emplace_back(std::make_shared<BeamSection>("Padrão", 0.06f, 0.00045f));
    sections.emplace_back(std::make_shared<BeamSection>("Retangular 20x40", 0.06f, 0.00045f));
    sections.emplace_back(std::make_shared<BeamSection>("Circular d30", 0.0706858f, 0.000398f));
}

template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::make_nvp("sections", sections));
    }
};

