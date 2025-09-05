/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** limited_cylinder
*/

#include "raytracer/limited_cylinder.hpp"
#include <cmath>

bool LimitedCylinder::hit_base(const Ray& r, double t_min, double t_max, HitRecord& rec, bool is_top_base) const {
    double denom = r.direction().dot(axis);
    if (std::abs(denom) < 0.0001) return false;

    const Math::Vector3D& center = is_top_base ? min_center : max_center;
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

bool LimitedCylinder::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    Math::Vector3D oc = r.origin() - base_center;
    
    Math::Vector3D axis_cross_dir = axis.cross(r.direction());
    Math::Vector3D axis_cross_oc = axis.cross(oc);
    
    double a = axis_cross_dir.length() * axis_cross_dir.length();
    double b = 2.0 * axis_cross_dir.dot(axis_cross_oc);
    double c = axis_cross_oc.length() * axis_cross_oc.length() - radius * radius;
    
    double discriminant = b * b - 4.0 * a * c;
    
    if (discriminant >= 0) {
        double sqrtd = sqrt(discriminant);
        double root = (-b - sqrtd) / (2.0 * a);
        
        if (root < t_min || t_max < root) {
            root = (-b + sqrtd) / (2.0 * a);
            if (root < t_min || t_max < root) {
            } else {
                Math::Vector3D hit_point = r.at(root);
                double dist_along_axis = (hit_point - base_center).dot(axis);
                
                if (dist_along_axis >= min_height && dist_along_axis <= max_height) {
                    Math::Vector3D hit_to_center = hit_point - base_center;
                    Math::Vector3D axis_projection = axis * hit_to_center.dot(axis);
                    Math::Vector3D radial = hit_to_center - axis_projection;
                    
                    if (radial.length() > 0.0001) {
                        Math::Vector3D normal = radial.normalized();
                        
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
            double dist_along_axis = (hit_point - base_center).dot(axis);
            
            if (dist_along_axis >= min_height && dist_along_axis <= max_height) {
                Math::Vector3D hit_to_center = hit_point - base_center;
                Math::Vector3D axis_projection = axis * hit_to_center.dot(axis);
                Math::Vector3D radial = hit_to_center - axis_projection;
                
                if (radial.length() > 0.0001) {
                    Math::Vector3D normal = radial.normalized();
                    
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