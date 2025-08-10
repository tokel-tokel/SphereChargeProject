#include "SphereRenderer.h"

SphereMesh SphereMesh::createSphere(size_t meridians, size_t parallels)
{
    if (meridians < 2 || parallels < 3) throw std::invalid_argument("Count of meridians must be at least 2 and of parallels at least 3");
    const size_t merSize{parallels - 2};
    SphereMesh mesh;
    mesh.vertices.insert(mesh.vertices.end(), {0.0f, 0.0f, 0.0f, 1.0f, 0.0f}); // North Pole
    mesh.vertices.insert(mesh.vertices.end(), {0.0f, pi, 0.0f, -1.0f, 0.0f}); // South Pole
    constexpr size_t nPole{0};
    constexpr size_t sPole{1};
    constexpr size_t begin{2};
    for (size_t mer{0}; mer < meridians; ++mer)
    {
        float phi{mer * pi / static_cast<float>(meridians)};
        for (size_t par{1}; par < parallels - 1; ++par)
        {
            float theta{calcTheta(par, parallels)};
            mesh.vertices.insert(mesh.vertices.end(), {phi, theta,
                glm::sin(phi)*glm::sin(theta), glm::cos(theta), glm::cos(phi)*glm::sin(theta)});
            if (par > 1 && par < parallels - 2)
            {
                mesh.indices.insert(mesh.indices.end(), {begin + mer * merSize + par - 1, begin + (mer + 1) % meridians * merSize + par - 2,
                    begin + mer * merSize + par - 2});
                mesh.indices.insert(mesh.indices.end(), {begin + mer * merSize + par - 1, begin + (mer + 1) % meridians * merSize + par - 1,
                    begin + (mer + 1) % meridians * merSize + par - 2});
            }
            else
            {
                if (par == 1)
                {
                    mesh.indices.insert(mesh.indices.end(), {begin + mer * merSize + par - 1, begin + (mer + 1) % meridians * merSize + par - 1,
                        nPole});
                }
                if (par == parallels - 2)
                {
                    mesh.indices.insert(mesh.indices.end(), {begin + mer * merSize + par - 1, sPole,
                        begin + (mer + 1) % meridians * merSize + par - 1});
                }
            }
        }
    }
    return mesh;
}

SphereRenderer::SphereRenderer(ChargedSphere& sphere, Shader&& shader, SphereMesh&& mesh) : sphere(sphere), shader(std::move(shader)),
    mesh(std::move(mesh))
{

}

