/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** limited_cone
*/

#include "raytracer/limited_cone.hpp"
#include <cmath>

bool LimitedCone::hit_base(const Ray& r, double t_min, double t_max, HitRecord& rec, bool is_top_base) const {
    double denom = r.direction().dot(axis);
    if (std::abs(denom) < 0.0001) return false;

    const Math::Vector3D& center = is_top_base ? min_center : base_center;
    const double& radius = is_top_base ? min_radius : max_radius;
    const Math::Vector3D& normal = is_top_base ? axis : -axis;

    double t = (center - r.origin()).dot(axis) / denom;
    if (t < t_min || t > t_max) return false;

    Math::Vector3D hit_point = r.at(t);
    
    Math::Vector3D to_hit = hit_point - center;
    double dist_squared = to_hit.length() * to_hit.length();
    if (dist_squared > radius * radius) return false;

    rec.t = t;
    rec.p = hit_point;
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;
    return true;
}

bool LimitedCone::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    Math::Vector3D co = r.origin() - apex;
    
    double cos2 = cos(angle) * cos(angle);
    
    double a = (r.direction().dot(axis) * r.direction().dot(axis)) - 
               (r.direction().length() * r.direction().length() * cos2);
    
    double b = 2.0 * ((r.direction().dot(axis) * co.dot(axis)) - 
                      (r.direction().dot(co) * cos2));
    
    double c = (co.dot(axis) * co.dot(axis)) - 
               (co.length() * co.length() * cos2);
    
    double discriminant = b * b - 4.0 * a * c;
    
    if (discriminant >= 0) {
        double sqrtd = sqrt(discriminant);
        double root = (-b - sqrtd) / (2.0 * a);
        
        if (root < t_min || t_max < root) {
            root = (-b + sqrtd) / (2.0 * a);
            if (root < t_min || t_max < root) {
                // Try the other root
            } else {
                Math::Vector3D hit_point = r.at(root);
                double dist_along_axis = (hit_point - apex).dot(axis);
                
                if (dist_along_axis >= min_height && dist_along_axis <= max_height) {
                    Math::Vector3D hit_to_apex = hit_point - apex;
                    Math::Vector3D axis_projection = axis * hit_to_apex.dot(axis);
                    Math::Vector3D radial = hit_to_apex - axis_projection;
                    
                    if (radial.length() > 0.0001) {
                        Math::Vector3D normal = (radial.normalized() * cos(angle) + axis * sin(angle)).normalized();
                        
                        if (normal.dot(r.direction()) > 0)
                            normal = -normal;
                        
                        temp_rec.t = root;
                        temp_rec.p = hit_point;
                        temp_rec.set_face_normal(r, normal);
                        temp_rec.mat_ptr = mat_ptr;
                        hit_anything = true;
                        closest_so_far = root;
                        rec = temp_rec;
                    }
                }
            }
        } else {
            Math::Vector3D hit_point = r.at(root);
            double dist_along_axis = (hit_point - apex).dot(axis);
            
            if (dist_along_axis >= min_height && dist_along_axis <= max_height) {
                Math::Vector3D hit_to_apex = hit_point - apex;
                Math::Vector3D axis_projection = axis * hit_to_apex.dot(axis);
                Math::Vector3D radial = hit_to_apex - axis_projection;
                
                if (radial.length() > 0.0001) {
                    Math::Vector3D normal = (radial.normalized() * cos(angle) + axis * sin(angle)).normalized();
                    
                    if (normal.dot(r.direction()) > 0)
                        normal = -normal;
                    
                    temp_rec.t = root;
                    temp_rec.p = hit_point;
                    temp_rec.set_face_normal(r, normal);
                    temp_rec.mat_ptr = mat_ptr;
                    hit_anything = true;
                    closest_so_far = root;
                    rec = temp_rec;
                }
            }
        }
    }

    if (hit_base(r, t_min, closest_so_far, temp_rec, true)) {
        hit_anything = true;
        rec = temp_rec;
        closest_so_far = temp_rec.t;
    }
    if (hit_base(r, t_min, closest_so_far, temp_rec, false)) {
        hit_anything = true;
        rec = temp_rec;
    }

    return hit_anything;
} 