/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** material
*/

#pragma once
#include "ray.hpp"
#include "math/vector3D.hpp"
#include "hittable.hpp"

class Material {
    public:
        virtual ~Material() = default;
        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Math::Vector3D& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
    public:
        Lambertian(const Math::Vector3D& a) : albedo(a) {}

        virtual bool scatter([[maybe_unused]] const Ray& r_in, const HitRecord& rec, Math::Vector3D& attenuation, Ray& scattered) const override {
            Math::Vector3D scatter_direction = rec.normal + Math::Vector3D::random_unit_vector();
            scattered = Ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        Math::Vector3D albedo;
};

class Metal : public Material {
    public:
        Metal(const Math::Vector3D& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Math::Vector3D& attenuation, Ray& scattered) const override {
            Math::Vector3D reflected = reflect(r_in.direction().normalized(), rec.normal);
            scattered = Ray(rec.p, reflected + fuzz*Math::Vector3D::random_in_unit_sphere());
            attenuation = albedo;
            return (scattered.direction().dot(rec.normal) > 0);
        }

    private:
        Math::Vector3D albedo;
        double fuzz;

        Math::Vector3D reflect(const Math::Vector3D& v, const Math::Vector3D& n) const {
            return v - 2*v.dot(n)*n;
        }
};

class Dielectric : public Material {
    public:
        Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(const Ray& r_in, const HitRecord& rec, Math::Vector3D& attenuation, Ray& scattered) const override {
            attenuation = Math::Vector3D(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

            Math::Vector3D unit_direction = r_in.direction().normalized();
            double cos_theta = std::min((-unit_direction).dot(rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Math::Vector3D direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > Math::random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = Ray(rec.p, direction);
            return true;
        }

    private:
        double ir;

        Math::Vector3D reflect(const Math::Vector3D& v, const Math::Vector3D& n) const {
            return v - 2*v.dot(n)*n;
        }

        Math::Vector3D refract(const Math::Vector3D& uv, const Math::Vector3D& n, double etai_over_etat) const {
            auto cos_theta = std::min((-uv).dot(n), 1.0);
            Math::Vector3D r_out_perp =  etai_over_etat * (uv + cos_theta*n);
            Math::Vector3D r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length()*r_out_perp.length())) * n;
            return r_out_perp + r_out_parallel;
        }

        static double reflectance(double cosine, double ref_idx) {
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};
