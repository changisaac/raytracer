#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() {};

    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    // Runs hit function for all objects in list and returns whether an object was hit at all
    // Populates rec with hit_record of closest hit object
    virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool HittableList::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const{
    double t_closest = t_max;
    bool hit_anything = false;
    hit_record temp_rec;


    for (const auto& object : objects) {
        // Set t_max to be t_closest so if hit is farther than a previous closer hit
        // then disregard hit since object is occluded
        // Below will return false if object further than t_max (t_closest)
        // If it is valid then a closer point was found so update t_closest
        if (object->hit(r, t_min, t_closest, temp_rec)) {
            hit_anything = true;
            t_closest = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif