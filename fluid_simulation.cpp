#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>

// Particle structure
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color; // Color of the particle
    float density;   // Density of the particle
};

const int WIDTH = 800;
const int HEIGHT = 600;
const int NUM_PARTICLES = 1000;

// Function to create particles
std::vector<Particle> createParticles() {
    std::vector<Particle> particles;

    // Create denser (black) particles
    for (int i = 0; i < NUM_PARTICLES / 2; ++i) {
        particles.push_back({
            {static_cast<float>(rand() % WIDTH), static_cast<float>(rand() % (HEIGHT / 2))},
            {0.f, 0.f},
            sf::Color::Black,
            2.0f  // Higher density
        });
    }

    // Create lighter (white) particles
    for (int i = 0; i < NUM_PARTICLES / 2; ++i) {
        particles.push_back({
            {static_cast<float>(rand() % WIDTH), static_cast<float>(HEIGHT / 2 + rand() % (HEIGHT / 2))},
            {0.f, 0.f},
            sf::Color::White,
            1.0f  // Lower density
        });
    }

    return particles;
}

// Function to update particles
void updateParticles(std::vector<Particle>& particles, float deltaTime) {
    const float gravity = 9.8f;
    const float interactionStrength = 0.001f;

    for (auto& p : particles) {
        // Apply gravity
        p.velocity.y += gravity * p.density * deltaTime;

        // Simple collision with boundaries
        if (p.position.x < 0 || p.position.x > WIDTH) p.velocity.x = -p.velocity.x;
        if (p.position.y < 0 || p.position.y > HEIGHT) p.velocity.y = -p.velocity.y;

        // Update position
        p.position += p.velocity * deltaTime;

        // Interaction between particles
        for (auto& other : particles) {
            if (&p == &other) continue;

            float dx = other.position.x - p.position.x;
            float dy = other.position.y - p.position.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < 10.0f) { // Interaction threshold
                float force = interactionStrength * (p.density - other.density) / dist;
                p.velocity.x += force * dx;
                p.velocity.y += force * dy;
            }
        }
    }
}

// Main function
int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fluid Simulation");

    std::vector<Particle> particles = createParticles();
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        // Update particles
        updateParticles(particles, deltaTime);

        // Render particles
        window.clear(sf::Color::Blue);

        for (const auto& p : particles) {
            sf::CircleShape shape(2.0f);
            shape.setPosition(p.position);
            shape.setFillColor(p.color);
            window.draw(shape);
        }

        window.display();
    }

    return 0;
}
