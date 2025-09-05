#include "raytracer/scene.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/triangle.hpp"
#include "raytracer/cone.hpp"
#include "raytracer/limited_cone.hpp"
#include "raytracer/limited_cylinder.hpp"
#include "raytracer/material.hpp"
#include <fstream>
#include <iostream>
#include "raytracer/rotation.hpp"
#include "raytracer/config.hpp"

Scene::Scene() : image_width(0), image_height(0), samples_per_pixel(0), max_depth(0),
                camera(Math::Vector3D(0, 0, 0), Math::Vector3D(0, 0, -1), Math::Vector3D(0, 1, 0), 
                       90.0, 1.0, 0.0, 1.0) {
}

bool Scene::loadFromFile(const std::string& filename) {
    Config config = load_config(filename);
    
    image_width = config.image.width;
    image_height = config.image.height;
    samples_per_pixel = config.image.samples;
    max_depth = config.image.max_depth;
    double aspect_ratio = static_cast<double>(image_width) / image_height;
    
    camera = Camera(
        config.camera.lookfrom,
        config.camera.lookat,
        config.camera.vup,
        config.camera.vfov,
        aspect_ratio,
        config.camera.aperture,
        config.camera.focus_dist
    );
    
    world = HittableList();
    
    SimpleConfig data = SimpleConfig::parse(filename);
    
    if (data.root.contains("objects")) {
        for (size_t i = 0; i < data.root["objects"].array.size(); i++) {
            auto& obj = data.root["objects"].array[i];
            
            if (!obj.contains("type")) continue;
            std::string type = obj["type"].asString();
            
            if (type == "sphere") {
                Math::Vector3D center;
                double radius = 1.0;
                Math::Vector3D translate(0, 0, 0);

                if (obj.contains("center")) {
                    center = Math::Vector3D(
                        obj["center"][0].asNumber(),
                        obj["center"][1].asNumber(),
                        obj["center"][2].asNumber()
                    );
                }
                
                if (obj.contains("radius")) {
                    radius = obj["radius"].asNumber();
                }

                if (obj.contains("translate")) {
                    translate = Math::Vector3D(
                        obj["translate"][0].asNumber(),
                        obj["translate"][1].asNumber(),
                        obj["translate"][2].asNumber()
                    );
                }
                
                std::shared_ptr<Material> material;
                
                if (obj.contains("material")) {
                    if (obj["material"].isString()) {
                        std::string material_name = obj["material"].asString();
                        if (config.materials.count(material_name)) {
                            MaterialProperties mat_props = config.materials[material_name];
                            
                            if (mat_props.type == "lambertian") {
                                material = std::make_shared<Lambertian>(mat_props.albedo);
                            } else if (mat_props.type == "metal") {
                                material = std::make_shared<Metal>(mat_props.albedo, mat_props.fuzz);
                            } else if (mat_props.type == "dielectric") {
                                material = std::make_shared<Dielectric>(mat_props.refraction_index);
                            }
                        }
                    } else if (obj["material"].isObject()) {
                        auto& matObj = obj["material"];
                        std::string mat_type = matObj.contains("type") ? matObj["type"].asString() : "lambertian";
                        
                        if (mat_type == "lambertian") {
                            Math::Vector3D albedo(0.5, 0.5, 0.5);
                            if (matObj.contains("albedo")) {
                                albedo = Math::Vector3D(
                                    matObj["albedo"][0].asNumber(),
                                    matObj["albedo"][1].asNumber(),
                                    matObj["albedo"][2].asNumber()
                                );
                            }
                            material = std::make_shared<Lambertian>(albedo);
                        } else if (mat_type == "metal") {
                            Math::Vector3D albedo(0.7, 0.7, 0.7);
                            double fuzz = 0.0;
                            
                            if (matObj.contains("albedo")) {
                                albedo = Math::Vector3D(
                                    matObj["albedo"][0].asNumber(),
                                    matObj["albedo"][1].asNumber(),
                                    matObj["albedo"][2].asNumber()
                                );
                            }
                            
                            if (matObj.contains("fuzz")) {
                                fuzz = matObj["fuzz"].asNumber();
                            }
                            
                            material = std::make_shared<Metal>(albedo, fuzz);
                        } else if (mat_type == "dielectric") {
                            double refraction_index = 1.5;
                            
                            if (matObj.contains("refraction_index")) {
                                refraction_index = matObj["refraction_index"].asNumber();
                            }
                            
                            material = std::make_shared<Dielectric>(refraction_index);
                        }
                    }
                }
                
                if (!material) {
                    material = std::make_shared<Lambertian>(Math::Vector3D(0.5, 0.5, 0.5));
                }

                center = center + translate;
                world.add(std::make_shared<Sphere>(center, radius, material));
            }

            if (type == "triangle") {
                Math::Vector3D left;
                Math::Vector3D right;
                Math::Vector3D top;
                Math::Vector3D translate(0, 0, 0);
                
                if (obj.contains("left")) {
                    left = Math::Vector3D(
                        obj["left"][0].asNumber(),
                        obj["left"][1].asNumber(),
                        obj["left"][2].asNumber()
                    );
                }

                if (obj.contains("right")) {
                    right = Math::Vector3D(
                        obj["right"][0].asNumber(),
                        obj["right"][1].asNumber(),
                        obj["right"][2].asNumber()
                    );
                }

                if (obj.contains("top")) {
                    top = Math::Vector3D(
                        obj["top"][0].asNumber(),
                        obj["top"][1].asNumber(),
                        obj["top"][2].asNumber()
                    );
                }

                if (obj.contains("translate")) {
                    translate = Math::Vector3D(
                        obj["translate"][0].asNumber(),
                        obj["translate"][1].asNumber(),
                        obj["translate"][2].asNumber()
                    );
                }
                
                std::shared_ptr<Material> material;
                
                if (obj.contains("material")) {
                    if (obj["material"].isString()) {
                        std::string material_name = obj["material"].asString();
                        if (config.materials.count(material_name)) {
                            MaterialProperties mat_props = config.materials[material_name];
                            
                            if (mat_props.type == "lambertian") {
                                material = std::make_shared<Lambertian>(mat_props.albedo);
                            } else if (mat_props.type == "metal") {
                                material = std::make_shared<Metal>(mat_props.albedo, mat_props.fuzz);
                            } else if (mat_props.type == "dielectric") {
                                material = std::make_shared<Dielectric>(mat_props.refraction_index);
                            }
                        }
                    } else if (obj["material"].isObject()) {
                        auto& matObj = obj["material"];
                        std::string mat_type = matObj.contains("type") ? matObj["type"].asString() : "lambertian";
                        
                        if (mat_type == "lambertian") {
                            Math::Vector3D albedo(0.5, 0.5, 0.5);
                            if (matObj.contains("albedo")) {
                                albedo = Math::Vector3D(
                                    matObj["albedo"][0].asNumber(),
                                    matObj["albedo"][1].asNumber(),
                                    matObj["albedo"][2].asNumber()
                                );
                            }
                            material = std::make_shared<Lambertian>(albedo);
                        } else if (mat_type == "metal") {
                            Math::Vector3D albedo(0.7, 0.7, 0.7);
                            double fuzz = 0.0;
                            
                            if (matObj.contains("albedo")) {
                                albedo = Math::Vector3D(
                                    matObj["albedo"][0].asNumber(),
                                    matObj["albedo"][1].asNumber(),
                                    matObj["albedo"][2].asNumber()
                                );
                            }
                            
                            if (matObj.contains("fuzz")) {
                                fuzz = matObj["fuzz"].asNumber();
                            }
                            
                            material = std::make_shared<Metal>(albedo, fuzz);
                        } else if (mat_type == "dielectric") {
                            double refraction_index = 1.5;
                            
                            if (matObj.contains("refraction_index")) {
                                refraction_index = matObj["refraction_index"].asNumber();
                            }
                            
                            material = std::make_shared<Dielectric>(refraction_index);
                        }
                    }
                }
                
                if (!material) {
                    material = std::make_shared<Lambertian>(Math::Vector3D(0.5, 0.5, 0.5));
                }

                left = left + translate;
                right = right + translate;
                top = top + translate;
                world.add(std::make_shared<Triangle>(left, right, top, material));

                if (obj.contains("rotate")) {
                    Math::Vector3D axis(0, 1, 0);
                    double angle = 0.0;

                    if (obj["rotate"].contains("axis")) {
                        axis = Math::Vector3D(
                            obj["rotate"]["axis"][0].asNumber(),
                            obj["rotate"]["axis"][1].asNumber(),
                            obj["rotate"]["axis"][2].asNumber()
                        );
                    }
                    if (obj["rotate"].contains("angle")) {
                        angle = obj["rotate"]["angle"].asNumber();
                    }
                    std::shared_ptr<Hittable> lastObject = world.get_last();
                    if (lastObject) {
                        world.replace_last(std::make_shared<Rotation>(lastObject, axis, angle));
                    }
                }
            }

            if (type == "cone") {
                Math::Vector3D apex;
                Math::Vector3D axis(0, 1, 0);
                double angle = 45.0;
                double height = 1.0;
                
                if (obj.contains("apex")) {
                    apex = Math::Vector3D(
                        obj["apex"][0].asNumber(),
                        obj["apex"][1].asNumber(),
                        obj["apex"][2].asNumber()
                    );
                }
                
                if (obj.contains("axis")) {
                    axis = Math::Vector3D(
                        obj["axis"][0].asNumber(),
                        obj["axis"][1].asNumber(),
                        obj["axis"][2].asNumber()
                    );
                }
                
                if (obj.contains("angle")) {
                    angle = obj["angle"].asNumber() * M_PI / 180.0;
                }
                
                if (obj.contains("height")) {
                    height = obj["height"].asNumber();
                }
                
                std::shared_ptr<Material> material;
                
                if (obj.contains("material")) {
                    if (obj["material"].isString()) {
                        std::string material_name = obj["material"].asString();
                        if (config.materials.count(material_name)) {
                            MaterialProperties mat_props = config.materials[material_name];
                            
                            if (mat_props.type == "lambertian") {
                                material = std::make_shared<Lambertian>(mat_props.albedo);
                            } else if (mat_props.type == "metal") {
                                material = std::make_shared<Metal>(mat_props.albedo, mat_props.fuzz);
                            } else if (mat_props.type == "dielectric") {
                                material = std::make_shared<Dielectric>(mat_props.refraction_index);
                            }
                        }
                    } else if (obj["material"].isObject()) {
                        auto& matObj = obj["material"];
                        std::string mat_type = matObj.contains("type") ? matObj["type"].asString() : "lambertian";
                        
                        if (mat_type == "lambertian") {
                            Math::Vector3D albedo(0.5, 0.5, 0.5);
                            if (matObj.contains("albedo")) {
                                albedo = Math::Vector3D(
                                    matObj["albedo"][0].asNumber(),
                                    matObj["albedo"][1].asNumber(),
                                    matObj["albedo"][2].asNumber()
                                );
                            }
                            material = std::make_shared<Lambertian>(albedo);
                        } else if (mat_type == "metal") {
                            Math::Vector3D albedo(0.7, 0.7, 0.7);
                            double fuzz = 0.0;
                            
                            if (matObj.contains("albedo")) {
                                albedo = Math::Vector3D(
                                    matObj["albedo"][0].asNumber(),
                                    matObj["albedo"][1].asNumber(),
                                    matObj["albedo"][2].asNumber()
                                );
                            }
                            
                            if (matObj.contains("fuzz")) {
                                fuzz = matObj["fuzz"].asNumber();
                            }
                            
                            material = std::make_shared<Metal>(albedo, fuzz);
                        } else if (mat_type == "dielectric") {
                            double refraction_index = 1.5;
                            
                            if (matObj.contains("refraction_index")) {
                                refraction_index = matObj["refraction_index"].asNumber();
                            }
                            
                            material = std::make_shared<Dielectric>(refraction_index);
                        }
                    }
                }
                
                if (!material) {
                    material = std::make_shared<Lambertian>(Math::Vector3D(0.5, 0.5, 0.5));
                }
                
                world.add(std::make_shared<Cone>(apex, axis, angle, height, material));
            }

            if (type == "limited_cone") {
                Math::Vector3D base_center;
                Math::Vector3D axis(0, 1, 0);
                double angle = 45.0;
                double min_height = 0.0;
                double max_height = 1.0;
                
                if (obj.contains("base_center")) {
                    base_center = Math::Vector3D(
                        obj["base_center"][0].asNumber(),
                        obj["base_center"][1].asNumber(),
                        obj["base_center"][2].asNumber()
                    );
                }
                
                if (obj.contains("axis")) {
                    axis = Math::Vector3D(
                        obj["axis"][0].asNumber(),
                        obj["axis"][1].asNumber(),
                        obj["axis"][2].asNumber()
                    );
                }
                
                if (obj.contains("angle")) {
                    angle = obj["angle"].asNumber() * M_PI / 180.0;
                }
                
                if (obj.contains("min_height")) {
                    min_height = obj["min_height"].asNumber();
                }
                
                if (obj.contains("max_height")) {
                    max_height = obj["max_height"].asNumber();
                }
                
                std::shared_ptr<Material> material;
                
                if (obj.contains("material")) {
                    if (obj["material"].isString()) {
                        std::string material_name = obj["material"].asString();
                        if (config.materials.count(material_name)) {
                            MaterialProperties mat_props = config.materials[material_name];
                            
                            if (mat_props.type == "lambertian") {
                                material = std::make_shared<Lambertian>(mat_props.albedo);
                            } else if (mat_props.type == "metal") {
                                material = std::make_shared<Metal>(mat_props.albedo, mat_props.fuzz);
                            } else if (mat_props.type == "dielectric") {
                                material = std::make_shared<Dielectric>(mat_props.refraction_index);
                            }
                        }
                    } else if (obj["material"].isObject()) {
                        auto& matObj = obj["material"];
                        std::string mat_type = matObj.contains("type") ? matObj["type"].asString() : "lambertian";
                        
                        if (mat_type == "lambertian") {
                            Math::Vector3D albedo(0.5, 0.5, 0.5);
                            if (matObj.contains("albedo")) {
                                albedo = Math::Vector3D(
                                    matObj["albedo"][0].asNumber(),
                                    matObj["albedo"][1].asNumber(),
                                    matObj["albedo"][2].asNumber()
                                );
                            }
                            material = std::make_shared<Lambertian>(albedo);
                        } else if (mat_type == "metal") {
                            Math::Vector3D albedo(0.7, 0.7, 0.7);
                            double fuzz = 0.0;
                            
                            if (matObj.contains("albedo")) {
                                albedo = Math::Vector3D(
                                    matObj["albedo"][0].asNumber(),
                                    matObj["albedo"][1].asNumber(),
                                    matObj["albedo"][2].asNumber()
                                );
                            }
                            
                            if (matObj.contains("fuzz")) {
                                fuzz = matObj["fuzz"].asNumber();
                            }
                            
                            material = std::make_shared<Metal>(albedo, fuzz);
                        } else if (mat_type == "dielectric") {
                            double refraction_index = 1.5;
                            
                            if (matObj.contains("refraction_index")) {
                                refraction_index = matObj["refraction_index"].asNumber();
                            }
                            
                            material = std::make_shared<Dielectric>(refraction_index);
                        }
                    }
                }
                
                if (!material) {
                    material = std::make_shared<Lambertian>(Math::Vector3D(0.5, 0.5, 0.5));
                }
                
                world.add(std::make_shared<LimitedCone>(base_center, axis, angle, min_height, max_height, material));
            }

            if (type == "limited_cylinder") {
                Math::Vector3D base_center;
                Math::Vector3D axis(0, 1, 0);
                double radius = 1.0;
                double min_height = 0.0;
                double max_height = 1.0;
                
                if (obj.contains("base_center")) {
                    base_center = Math::Vector3D(
                        obj["base_center"][0].asNumber(),
                        obj["base_center"][1].asNumber(),
                        obj["base_center"][2].asNumber()
                    );
                }
                
                if (obj.contains("axis")) {
                    axis = Math::Vector3D(
                        obj["axis"][0].asNumber(),
                        obj["axis"][1].asNumber(),
                        obj["axis"][2].asNumber()
                    );
                }
                
                if (obj.contains("radius")) {
                    radius = obj["radius"].asNumber();
                }
                
                if (obj.contains("min_height")) {
                    min_height = obj["min_height"].asNumber();
                }
                
                if (obj.contains("max_height")) {
                    max_height = obj["max_height"].asNumber();
                }
                
                std::shared_ptr<Material> material;
                if (obj.contains("material")) {
                    if (obj["material"].isString()) {
                        std::string mat_name = obj["material"].asString();
                        if (config.materials.find(mat_name) != config.materials.end()) {
                            auto& mat_props = config.materials[mat_name];
                            if (mat_props.type == "lambertian") {
                                material = std::make_shared<Lambertian>(mat_props.albedo);
                            } else if (mat_props.type == "metal") {
                                material = std::make_shared<Metal>(mat_props.albedo, mat_props.fuzz);
                            } else if (mat_props.type == "dielectric") {
                                material = std::make_shared<Dielectric>(mat_props.refraction_index);
                            }
                        }
                    } else if (obj["material"].isObject()) {
                        auto& matObj = obj["material"];
                        std::string mat_type = matObj["type"].asString();
                        
                        if (mat_type == "lambertian") {
                            Math::Vector3D albedo(0.5, 0.5, 0.5);
                            
                            if (matObj.contains("albedo")) {
                                albedo = Math::Vector3D(
                                    matObj["albedo"][0].asNumber(),
                                    matObj["albedo"][1].asNumber(),
                                    matObj["albedo"][2].asNumber()
                                );
                            }
                            
                            material = std::make_shared<Lambertian>(albedo);
                        } else if (mat_type == "metal") {
                            Math::Vector3D albedo(0.7, 0.7, 0.7);
                            double fuzz = 0.0;
                            
                            if (matObj.contains("albedo")) {
                                albedo = Math::Vector3D(
                                    matObj["albedo"][0].asNumber(),
                                    matObj["albedo"][1].asNumber(),
                                    matObj["albedo"][2].asNumber()
                                );
                            }
                            
                            if (matObj.contains("fuzz")) {
                                fuzz = matObj["fuzz"].asNumber();
                            }
                            
                            material = std::make_shared<Metal>(albedo, fuzz);
                        } else if (mat_type == "dielectric") {
                            double refraction_index = 1.5;
                            
                            if (matObj.contains("refraction_index")) {
                                refraction_index = matObj["refraction_index"].asNumber();
                            }
                            
                            material = std::make_shared<Dielectric>(refraction_index);
                        }
                    }
                }
                
                if (!material) {
                    material = std::make_shared<Lambertian>(Math::Vector3D(0.5, 0.5, 0.5));
                }
                
                world.add(std::make_shared<LimitedCylinder>(base_center, axis, radius, min_height, max_height, material));
            }
        }
    }
    
    return true;
}

Math::Vector3D Scene::readVector(const std::vector<double>& values) {
    if (values.size() >= 3) {
        return Math::Vector3D(values[0], values[1], values[2]);
    }
    return Math::Vector3D(0, 0, 0);
}

Math::Vector3D Scene::readColor(const std::vector<double>& values) {
    if (values.size() >= 3) {
        return Math::Vector3D(values[0], values[1], values[2]);
    }
    return Math::Vector3D(0, 0, 0);
} 