/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** config
*/

#pragma once
#include "math/vector3D.hpp"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

struct SimpleValue {
    enum Type { NULL_VALUE, NUMBER, STRING, BOOL, ARRAY, OBJECT };
    
    Type type = NULL_VALUE;
    double number_value = 0.0;
    std::string string_value;
    bool bool_value = false;
    std::vector<SimpleValue> array;
    std::map<std::string, SimpleValue> object;
    
    SimpleValue() = default;
    SimpleValue(double num) : type(NUMBER), number_value(num) {}
    SimpleValue(const std::string& str) : type(STRING), string_value(str) {}
    SimpleValue(bool b) : type(BOOL), bool_value(b) {}
    
    bool isNumber() const { return type == NUMBER; }
    bool isString() const { return type == STRING; }
    bool isBool() const { return type == BOOL; }
    bool isArray() const { return type == ARRAY; }
    bool isObject() const { return type == OBJECT; }
    
    double asNumber() const { return number_value; }
    const std::string& asString() const { return string_value; }
    bool asBool() const { return bool_value; }
    
    SimpleValue& operator[](const std::string& key) {
        type = OBJECT;
        return object[key];
    }
    
    SimpleValue& operator[](size_t index) {
        type = ARRAY;
        if (index >= array.size())
            array.resize(index + 1);
        return array[index];
    }
    
    bool contains(const std::string& key) const {
        return type == OBJECT && object.find(key) != object.end();
    }
};

struct SimpleConfig {
    SimpleValue root;
    
    static SimpleConfig parse(const std::string& filename) {
        SimpleConfig config;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << filename << std::endl;
            return config;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)), 
                            std::istreambuf_iterator<char>());
        
        size_t pos = 0;
        config.root = parseValue(content, pos);
        
        return config;
    }
    
private:
    static SimpleValue parseValue(const std::string& str, size_t& pos) {
        while (pos < str.size() && std::isspace(str[pos])) pos++;
        
        if (pos >= str.size()) return SimpleValue();
        
        char c = str[pos];
        
        if (c == '{') {
            return parseObject(str, pos);
        } else if (c == '[') {
            return parseArray(str, pos);
        } else if (c == '"') {
            return parseString(str, pos);
        } else if (std::isdigit(c) || c == '-') {
            return parseNumber(str, pos);
        } else if (c == 't' || c == 'f') {
            return parseBool(str, pos);
        }
        
        return SimpleValue();
    }
    
    static SimpleValue parseObject(const std::string& str, size_t& pos) {
        SimpleValue result;
        result.type = SimpleValue::OBJECT;
        
        pos++;
        
        while (pos < str.size()) {
            while (pos < str.size() && std::isspace(str[pos])) pos++;
            
            if (pos >= str.size()) break;
            
            if (str[pos] == '}') {
                pos++;
                break;
            }
            
            if (str[pos] == ',') {
                pos++;
                continue;
            }
            
            SimpleValue key = parseString(str, pos);
            
            while (pos < str.size() && std::isspace(str[pos])) pos++;
            
            if (pos < str.size() && str[pos] == ':') pos++;
            
            SimpleValue value = parseValue(str, pos);
            
            result.object[key.string_value] = value;
        }
        
        return result;
    }
    
    static SimpleValue parseArray(const std::string& str, size_t& pos) {
        SimpleValue result;
        result.type = SimpleValue::ARRAY;
        
        pos++;
        
        while (pos < str.size()) {
            while (pos < str.size() && std::isspace(str[pos])) pos++;
            
            if (pos >= str.size()) break;
            
            if (str[pos] == ']') {
                pos++;
                break;
            }
            
            if (str[pos] == ',') {
                pos++;
                continue;
            }
            
            SimpleValue value = parseValue(str, pos);
            
            result.array.push_back(value);
        }
        
        return result;
    }
    
    static SimpleValue parseString(const std::string& str, size_t& pos) {
        SimpleValue result;
        result.type = SimpleValue::STRING;
        
        pos++;
        
        size_t start = pos;
        while (pos < str.size() && str[pos] != '"') pos++;
        
        result.string_value = str.substr(start, pos - start);
        
        if (pos < str.size()) pos++;
        
        return result;
    }
    
    static SimpleValue parseNumber(const std::string& str, size_t& pos) {
        SimpleValue result;
        result.type = SimpleValue::NUMBER;
        
        size_t start = pos;
        while (pos < str.size() && (std::isdigit(str[pos]) || str[pos] == '.' || str[pos] == '-' || str[pos] == 'e' || str[pos] == 'E' || str[pos] == '+')) pos++;
        
        std::string num_str = str.substr(start, pos - start);
        result.number_value = std::stod(num_str);
        
        return result;
    }
    
    static SimpleValue parseBool(const std::string& str, size_t& pos) {
        SimpleValue result;
        result.type = SimpleValue::BOOL;
        
        if (str.substr(pos, 4) == "true") {
            result.bool_value = true;
            pos += 4;
        } else if (str.substr(pos, 5) == "false") {
            result.bool_value = false;
            pos += 5;
        }
        
        return result;
    }
};

struct MaterialProperties {
    std::string type;
    Math::Vector3D albedo;
    double fuzz = 0.0;
    double refraction_index = 1.0;
};

struct ImageConfig {
    int width = 800;
    int height = 600;
    int samples = 100;
    int max_depth = 50;
};

struct CameraConfig {
    Math::Vector3D lookfrom = Math::Vector3D(0, 0, 0);
    Math::Vector3D lookat = Math::Vector3D(0, 0, -1);
    Math::Vector3D vup = Math::Vector3D(0, 1, 0);
    double vfov = 90.0;
    double aperture = 0.0;
    double focus_dist = 1.0;
};

struct Config {
    ImageConfig image;
    CameraConfig camera;
    std::map<std::string, MaterialProperties> materials;
};

Config load_config(const std::string& filename) {
    Config config;
    SimpleConfig data = SimpleConfig::parse(filename);
    
    if (data.root.contains("image")) {
        auto& img = data.root["image"];
        if (img.contains("width")) config.image.width = img["width"].asNumber();
        if (img.contains("height")) config.image.height = img["height"].asNumber();
        if (img.contains("samples_per_pixel")) config.image.samples = img["samples_per_pixel"].asNumber();
        if (img.contains("max_depth")) config.image.max_depth = img["max_depth"].asNumber();
    }
    
    if (data.root.contains("camera")) {
        auto& cam = data.root["camera"];
        if (cam.contains("lookfrom")) {
            config.camera.lookfrom = Math::Vector3D(
                cam["lookfrom"][0].asNumber(),
                cam["lookfrom"][1].asNumber(),
                cam["lookfrom"][2].asNumber()
            );
        }
        if (cam.contains("lookat")) {
            config.camera.lookat = Math::Vector3D(
                cam["lookat"][0].asNumber(),
                cam["lookat"][1].asNumber(),
                cam["lookat"][2].asNumber()
            );
        }
        if (cam.contains("vup")) {
            config.camera.vup = Math::Vector3D(
                cam["vup"][0].asNumber(),
                cam["vup"][1].asNumber(),
                cam["vup"][2].asNumber()
            );
        }
        if (cam.contains("vfov")) config.camera.vfov = cam["vfov"].asNumber();
        if (cam.contains("aperture")) config.camera.aperture = cam["aperture"].asNumber();
        if (cam.contains("focus_dist")) config.camera.focus_dist = cam["focus_dist"].asNumber();
    }
    
    if (data.root.contains("materials")) {
        auto& mats = data.root["materials"];
        for (auto& pair : mats.object) {
            MaterialProperties mat;
            auto& matData = pair.second;
            
            if (matData.contains("type")) mat.type = matData["type"].asString();
            
            if (matData.contains("albedo")) {
                mat.albedo = Math::Vector3D(
                    matData["albedo"][0].asNumber(),
                    matData["albedo"][1].asNumber(),
                    matData["albedo"][2].asNumber()
                );
            }
            
            if (matData.contains("fuzz")) mat.fuzz = matData["fuzz"].asNumber();
            if (matData.contains("refraction_index")) mat.refraction_index = matData["refraction_index"].asNumber();
            
            config.materials[pair.first] = mat;
        }
    }
    
    return config;
}
