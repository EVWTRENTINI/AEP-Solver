#pragma once

class BeamMaterial {
public:
std::string name;
float E;
float alpha;

BeamMaterial() : name(""), E(0), alpha(0) {}

BeamMaterial(const char* n, float YM, float a) : name(n), E(YM), alpha(a) {}

template<class Archive>
void serialize(Archive& ar) {
    ar(cereal::make_nvp("name", name),
       cereal::make_nvp("E", E),
       cereal::make_nvp("alpha", alpha)
       );
}
};

class MaterialManager {
public:
std::vector<std::shared_ptr<BeamMaterial>> materials;

MaterialManager() {
    materials.emplace_back(std::make_shared<BeamMaterial>("Padrão", 2500E6, 0.000010f));
    materials.emplace_back(std::make_shared<BeamMaterial>("Concreto C20", 2500E6, 0.000010f));
    materials.emplace_back(std::make_shared<BeamMaterial>("Aço", 200E9, 0.000012f));
}

template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::make_nvp("materials", materials));
    }
};

