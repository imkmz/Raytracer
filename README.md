
# Raytracer – B-OOP-400

## Description

Ce projet a pour objectif de créer un moteur de rendu en **ray tracing** écrit en **C++**. Il génère une image réaliste en simulant le trajet inverse de la lumière, à partir d’un fichier de configuration de scène.

## Usage

```bash
./raytracer <SCENE_FILE>
```

- `<SCENE_FILE>` : fichier de configuration de la scène (ex: `.json`)

## Compilation

Le projet peut être compilé avec un `Makefile`.
- Le binaire `raytracer` doit être généré à la racine du dépôt.

## Fonctionnalités

### Obligatoires (Must)
- [ ] Primitives :
  - [x] Sphere
  - [ ] Plane
- [ ] Transformations :
  - [ ] Translation
- [ ] Lumières :
  - [ ] Directionnelle
  - [ ] Ambiante
- [ ] Matériaux :
  - [ ] Couleur unie (flat color)
- [ ] Scène :
  - [ ] Ajout de primitives
  - [ ] Configuration de la lumière et de la caméra
- [ ] Sortie :
  - [ ] Fichier image PPM (pas d’interface graphique)

### Optionnelles (Should)
- [ ] Primitives :
  - [ ] Cone
  - [ ] Cylinder
- [ ] Transformation :
  - [ ] Rotation
- [ ] Ombres portées :
  - [ ] Oui

### Avancées (Could)
- [ ] Primitives :
  - [ ] Torus
  - [ ] Triangle
  - [ ] .OBJ
  - [ ] Fractals, etc.
- [ ] Lumières :
  - [ ] Multiples sources
  - [ ] Lumière colorée
  - [ ] Phong
  - [ ] Occlusion ambiante
- [ ] Matériaux :
  - [ ] Réflexion
  - [ ] Réfraction
  - [ ] Textures (fichier, procédurale)
- [ ] Antialiasing :
  - [ ] Supersampling
  - [ ] Adaptatif
- [ ] Optimisation :
  - [ ] Partitionnement spatial
  - [ ] Multithreading
- [ ] Interface :
  - [ ] Affichage en direct
  - [ ] Preview rapide
  - [ ] Rechargement auto

## Format du fichier de scène

Fichier de configuration recommandé : `libconfig++`. Exemple :
```json
{
  "image": {
    "width": 1200,
    "height": 800,
    "samples_per_pixel": 100,
    "max_depth": 50
  },
  "camera": {
    "lookfrom": [13, 2, 3],
    "lookat": [0, 0, 0],
    "vup": [0, 1, 0],
    "vfov": 20.0,
    "aperture": 0.1,
    "focus_dist": 10.0
  },
  "objects": [
    {
      "type": "sphere",
      "center": [0, -1000, 0],
      "radius": 1000,
      "material": {
        "type": "lambertian",
        "albedo": [0.5, 0.5, 0.5]
      }
    }
  ]
}
```

## Architecture

- Utilisation d’interfaces pour les primitives et lumières
- Utilisation d’au moins **2 design patterns** parmi : Factory, Builder, Composite, Decorator, Observer, State, Mediator

---