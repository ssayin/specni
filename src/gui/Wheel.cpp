#include "Wheel.h"
#include "SDL_video.h"
#include "ImGuiExt.hpp"
#include "ZodiacsFont.h"
#include "ChartSettings.h"
#include "Math.h"
#include <imgui.h>
#include <tuple>
#include <utility>

namespace specni::gui {


void Wheel::Show() const {
	struct Constraint {

		static void Square(ImGuiSizeCallbackData *data) {
			data->DesiredSize =
					ImVec2(std::max(data->DesiredSize.x, data->DesiredSize.y),
							std::max(data->DesiredSize.x, data->DesiredSize.y));
		}
	};

	ImGui::SetNextWindowSizeConstraints(ImVec2(600, 600), ImVec2(800, 800),
			Constraint::Square);

	ImGui::Begin("Chart");
	ImDrawList *draw_list = ImGui::GetWindowDrawList();
	// const ImVec2 p = ImGui::GetCursorScreenPos();
	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetWindowSize();
	ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f,
			window_pos.y + window_size.y * 0.5f);

	std::array<ImVec2, 5> RPoints = getRPoints(window_size);

	ImVec2 RAscMc =
			(RPoints[ChartSettings::CuspText] + RPoints[ChartSettings::SignOuter]) /
					2;
	ImVec2 RHouseNumber = (RPoints[ChartSettings::Innermost] +
			RPoints[ChartSettings::CircleHouseNumbers]) /
			2;
	ImVec2 RSign =
			(RPoints[ChartSettings::SignInner] + RPoints[ChartSettings::SignOuter]) /
					2;

	drawCircles(draw_list, window_center, RPoints);

	ImGui::PushFont(settings.font);

	drawSignGlyphs(draw_list, window_center, RPoints, RSign);
	drawPlanetGlyphs(draw_list, window_size, window_center);


	char f[3];

auto [CosA, SinA] = degree::cosSinFixAt(model.houses.ang.ac,model.houses.ang.ac);

draw_list->AddLine(
			window_center + ImRotate(RPoints[ChartSettings::SignOuter], CosA, SinA),
			window_center + ImRotate(RPoints[ChartSettings::Innermost], CosA, SinA),
			settings.AscMcColor, settings.Thickness);

	sprintf(f, "%c", 'K');
	draw_list->AddText(window_center + ImRotate(RAscMc, CosA, SinA),
			settings.AscMcColor, f);

	std::tie(CosA, SinA) = degree::cosSinFixAt(model.houses.ang.mc, model.houses.ang.ac);

	draw_list->AddLine(
			window_center + ImRotate(RPoints[ChartSettings::SignOuter], CosA, SinA),
			window_center + ImRotate(RPoints[ChartSettings::Innermost], CosA, SinA),
			settings.AscMcColor, settings.Thickness);

	sprintf(f, "%c", 'L');

	draw_list->AddText(window_center + ImRotate(RAscMc, CosA, SinA),
			settings.AscMcColor, f);

	std::vector<ImVec2> vMidpoints;

	std::size_t size = model.houses.cusps.size();
	// Draw house cusps
	for (std::vector<float>::size_type i = 1; i < size;
		 ++i) {

		std::tie(CosA, SinA) = degree::cosSinFixAt(model.houses.cusps.at(i), model.houses.ang.ac);

		draw_list->AddLine(
				window_center + ImRotate(RPoints[ChartSettings::Innermost], CosA, SinA),
				window_center + ImRotate(RPoints[ChartSettings::SignInner], CosA, SinA),
				settings.BaseColor, settings.Thickness);

		vMidpoints.push_back(window_center + ImRotate(RHouseNumber, CosA, SinA));
	}

	for (std::vector<ImVec2>::size_type i = 1; i < vMidpoints.size(); ++i) {
		ImVec2 houseNumberPos = (vMidpoints.at(i) + vMidpoints.at(i - 1)) / 2.0;
		sprintf(f, "%d", static_cast<int>(i));
		draw_list->AddText(houseNumberPos, settings.AscMcColor, f);
	}

	if (!vMidpoints.empty()) {
		ImVec2 houseNumberPos =
				(vMidpoints.at(vMidpoints.size() - 1) + vMidpoints.at(0)) / 2.0;
		sprintf(f, "%d", static_cast<int>(vMidpoints.size()));
		draw_list->AddText(houseNumberPos, settings.AscMcColor, f);
	}

	ImGui::PopFont();

	/*for (std::tuple<swephpp::Ipl, swephpp::Ipl, Aspect, double, AspectStat> &x :
			model->vAspects) {
		float CosB;
		float SinB;

		std::tie(CosA, SinA) =
				GetDegreeCosSinRotAsc(model->Eph[std::get<0>(x)].Data.lon);
		std::tie(CosB, SinB) =
				GetDegreeCosSinRotAsc(model->Eph[std::get<1>(x)].Data.lon);

		draw_list->AddLine(
				window_center + ImRotate(RPoints[ChartSettings::Innermost], CosA, SinA),
				window_center + ImRotate(RPoints[ChartSettings::Innermost], CosB, SinB),
				ImColor(aspectColor[static_cast<size_t>(std::get<2>(x))]));
	}*/

	ImGui::End();

}
void Wheel::drawPlanetGlyphs(ImDrawList* draw_list, const ImVec2& window_size, const ImVec2& window_center) const
{
	ImVec2 mid2 = ImVec2(0, window_size.y * 0.32f);
	for (auto &p : model.planets.m) {
		auto find = PlanetCharMap.find(p.first);
		if (find != PlanetCharMap.end()) {
			char f[3];
			sprintf(f, "%c", find->second.first);
		auto [CosA, SinA] = degree::cosSinFixAt(p.second.longitude(), model.houses.ang.ac);
			draw_list->AddText(window_center + ImRotate(mid2, CosA, SinA),
					ImColor(find->second.second), f);
		}
	}
}
void Wheel::drawSignGlyphs(ImDrawList* draw_list, const ImVec2& window_center, const std::array<ImVec2, 5>& RPoints,
		const ImVec2& RSign) const
{
	constexpr int SignCount = 12;
	constexpr int SignSpanDegrees = 30;
	constexpr int HalfSignSpanDegrees = SignSpanDegrees / 2;

	for (int i = 0; i < SignCount; i++) {
		auto [CosA, SinA] = degree::cosSinFixAt(i * SignSpanDegrees, model.houses.ang.ac);

		draw_list->AddLine(
				window_center + ImRotate(RPoints[ChartSettings::SignInner], CosA, SinA),
				window_center + ImRotate(RPoints[ChartSettings::SignOuter], CosA, SinA),
				settings.BaseColor, settings.Thickness);

		char f[3];
		sprintf(f, "%c", 'a' + i);

		std::tie(CosA, SinA)  =
				degree::cosSinFixAt(i * SignSpanDegrees + HalfSignSpanDegrees, model.houses.ang.ac);

		draw_list->AddText(window_center + ImRotate(RSign, CosA, SinA),
				settings.SignColor, f);
	}
}
std::array<ImVec2, 5> Wheel::getRPoints(const ImVec2& window_size) const
{
	std::array<ImVec2, ChartSettings::Count> RPoints;

	for (int i = 0; i < ChartSettings::Count; ++i) {
		RPoints[i] = ImVec2(0, window_size.y * settings.Ratios[i]);
	}
	return RPoints;
}
void Wheel::drawCircles(ImDrawList* draw_list, const ImVec2& window_center,
		const std::array<ImVec2, ChartSettings::Count>& RPoints) const
{// Draw circles
	draw_list->AddCircle(window_center, RPoints[ChartSettings::SignOuter].y,
			settings.BaseColor, 0, settings.Thickness);

	draw_list->AddCircle(window_center, RPoints[ChartSettings::SignInner].y,
			settings.BaseColor, 0, settings.Thickness);

	draw_list->AddCircle(window_center, RPoints[ChartSettings::Innermost].y,
			settings.BaseColor, 0, settings.Thickness);

	draw_list->AddCircle(window_center,
			RPoints[ChartSettings::CircleHouseNumbers].y,
			settings.BaseColor, 0, settings.Thickness);
}
Wheel::Wheel(ChartSettings settings, core::Chart& model) : settings(settings), model(model)
{

}


}