#include <tardigrade/tardigrade.hpp>
#include <vulkify/vulkify.hpp>

namespace {
// set up RenderTarget: this will be passed in each RenderAttachment::render() call
struct Frame : vf::Frame, tg::RenderTarget {
	Frame(vf::Frame&& frame) { static_cast<vf::Frame&>(*this) = std::move(frame); }
};

// set up Renderer: owns a Mesh and renders it every frame
struct Renderer : tg::RenderAttachment {
	vf::Mesh mesh{};

	void setup() override {
		RenderAttachment::setup();
		mesh = vf::Mesh{tg::locate<vf::Context*>()->device()};
	}

	void tick(tg::DeltaTime) override {}
	void render(tg::RenderTarget const& target) const override { static_cast<Frame const&>(target).draw(mesh); }
};

// set up Controller: listens for keyboard input and positions attached Renderer instance accordingly
struct Controller : tg::TickAttachment {
	tg::Ptr<Renderer> renderer{};
	float speed{500.0f};

	glm::vec2 get_dxy() const {
		auto dxy = glm::vec2{};
		using vf::keyboard::held;
		if (held(vf::Key::eW) || held(vf::Key::eUp)) { dxy.y += 1.0f; }
		if (held(vf::Key::eD) || held(vf::Key::eRight)) { dxy.x += 1.0f; }
		if (held(vf::Key::eS) || held(vf::Key::eDown)) { dxy.y -= 1.0f; }
		if (held(vf::Key::eA) || held(vf::Key::eLeft)) { dxy.x -= 1.0f; }
		if (dxy.x != 0.0f || dxy.y != 0.0f) { dxy = glm::normalize(dxy); }
		return dxy;
	}

	void setup() override { renderer = entity()->find_or_attach<Renderer>(); }

	void tick(tg::DeltaTime dt) override {
		auto const dxy = get_dxy();
		renderer->mesh.instance().transform.position += dxy * dt.scaled.count() * speed;
	}
};

struct Quit : tg::TickAttachment {
	void setup() override {}
	void tick(tg::DeltaTime) override {
		if (vf::keyboard::pressed(vf::Key::eEscape)) { tg::locate<vf::Context*>()->close(); }
	}
};
} // namespace

int main() {
	auto result = vf::Builder{}.build();
	if (!result) { return EXIT_FAILURE; }

	// create a scoped Services instance (clears all attached services on destruction)
	auto services = tg::Services::Scoped{};
	// create a service provider for the context
	auto context = tg::ServiceProvider<vf::Context>{std::move(*result)};

	// create a scene director (optional)
	auto director = tg::Director{};
	// create a scene
	auto& scene = director.enqueue<tg::Scene>();
	// attach a new entity to the scene, and attach a Renderer and Controller instance to it
	auto* entity = scene.spawn<Renderer, Controller>();
	// set up the mesh in the Renderer instance
	entity->find<Renderer>()->mesh.buffer.write(vf::Geometry::make_quad());

	// spawn another entity and attach Quit to it
	scene.spawn<Quit>();

	// start looping
	context.show();
	while (!context.closing()) {
		// make frame
		auto frame = Frame{context.frame()};
		// tick scene
		director.tick(frame.dt());
		// render scene
		director.render(frame);
	}
}
