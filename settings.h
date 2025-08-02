#pragma once

#include <vector>
#include <iostream>
#include <filesystem>

#include <QSettings>
#include <QColor>
#include <QUrl>
#include <QApplication>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>

#include "types.h"

namespace DS {
class Settings : public QSettings
{
    Q_OBJECT
        Q_PROPERTY(bool showAxis READ showAxis WRITE setShowAxis NOTIFY showAxisChanged)
        QML_ELEMENT
public:
    Settings(QObject* parent = nullptr);

    bool showAxis() const;
    void setShowAxis(bool show);
    bool defaultShowAxis() const;

    //===========================
    struct interactor {
        bool invert_zoom = false;
        bool trackball = false;
    } interactor;

    struct model {
        struct color {
            std::optional<double> opacity;
            std::optional<f3d::color_t> rgb;
            std::optional<std::filesystem::path> texture;
        } color;

        struct emissive {
            std::optional<f3d::color_t> factor;
            std::optional<std::filesystem::path> texture;
        } emissive;

        struct matcap {
            std::optional<std::filesystem::path> texture;
        } matcap;

        struct material {
            std::optional<double> base_ior;
            std::optional<double> metallic;
            std::optional<double> roughness;
            std::optional<std::filesystem::path> texture;
        } material;

        struct normal {
            std::optional<double> scale;
            std::optional<std::filesystem::path> texture;
        } normal;

        struct point_sprites {
            bool enable = false;
            double size = 10.0;
            std::string type = "sphere";
        } point_sprites;

        struct scivis {
            std::optional<std::string> array_name;
            bool cells = false;
            f3d::colormap_t colormap = f3d::colormap_t{ 0.0, 0.0, 0.0, 0.0, 0.4, 0.9, 0.0, 0.0, 0.8, 0.9, 0.9, 0.0, 1.0, 1.0, 1.0, 1.0 };
            int component = -1;
            std::optional<int> discretization;
            bool enable = false;
            std::optional<std::vector<double>> range;
        } scivis;

        std::optional<f3d::transform2d_t> textures_transform;
        struct volume {
            bool enable = false;
            bool inverse = false;
        } volume;

    } model;

    struct render {
        struct armature {
            bool enable = false;
        } armature;

        struct axes_grid {
            bool enable = false;
        } axes_grid;

        std::optional<std::string> backface_type;
        struct background {
            struct blur {
                double coc = 20.0;
                bool enable = false;
            } blur;

            f3d::color_t color = f3d::color_t{ 0.2, 0.2, 0.2 };
            bool skybox = false;
        } background;

        struct effect {
            bool ambient_occlusion = false;
            [[deprecated("use render.effect.antialiasing.enable instead")]] bool anti_aliasing = false;
            struct antialiasing {
                bool enable = false;
                std::string mode = "fxaa";
            } antialiasing;

            std::optional<std::string> final_shader;
            bool tone_mapping = false;
            bool translucency_support = false;
        } effect;

        struct grid {
            bool absolute = false;
            f3d::color_t color = f3d::color_t{ 0.0, 0.0, 0.0 };
            bool enable = false;
            int subdivisions = 10;
            std::optional<double> unit;
        } grid;

        struct hdri {
            bool ambient = false;
            std::optional<std::filesystem::path> file;
        } hdri;

        struct light {
            double intensity = 1.0;
        } light;

        std::optional<double> line_width;
        std::optional<double> point_size;
        struct raytracing {
            bool denoise = false;
            bool enable = false;
            int samples = 5;
        } raytracing;

        std::optional<bool> show_edges;
    } render;

    struct scene {
        struct animation {
            bool autoplay = false;
            [[deprecated("use scene.animation.indices instead")]] int index = 0;
            std::vector<int> indices = { 0 };
            f3d::ratio_t speed_factor = f3d::ratio_t{ 1.0 };
        } animation;

        struct camera {
            std::optional<int> index;
            std::optional<bool> orthographic;
        } camera;

        std::optional<std::string> force_reader;
        f3d::direction_t up_direction = f3d::direction_t{ 0,1,0 };
    } scene;
    //=========================
    Q_INVOKABLE void storeIt();
signals:
    void showAxisChanged();
};
}