#include "Application.h"

Application::Application() {


	if (EDImage::TryLoad("C:/Users/adrianjmejias/Desktop/CG3-P1/chikis.jpg", img))
	{
		std::cout << "img loaded successfully\n";

		if (EDImage::TrySave(img, "saved.png"))
		{
			std::cout << "success img save \n";
		}
		else 
		{
			std::cout << "failed img save \n";
		}
	}
	else
	{
		std::cout << "img load fail\n";

	}

	//{
	//	std::unique_ptr<EDBaseConvolution> mv10{ EDConvolution::CreateCustom({ 0,0,0,0,0,0,0,0,0,1 }, 10, 1, 9, 0) };
	//	std::unique_ptr<EDImage> mvImg{ mv10->ApplyConvolution(*img) };
	//	Save(mvImg.get(), "mv.png");
	//}




	bw = EDConvolution::CreateNegative();
	composite = EDCompositeConv::Create();

	std::unique_ptr<EDConvolution> blur{ EDConvolution::CreateCustom(
		EDConvolution::Normalize({
			1,2,1,
			2,3,2,
			1,2,1
		}),
		{ 0,0,0,0 }, 3, 3, 1, 1) };

	std::unique_ptr<EDConvolution> blurPower{ EDConvolution::CreateCustom(
	EDConvolution::Normalize({
		1,2,3,2,1,
		2,4,6,4,2,
		3,6,8,6,3,
		2,4,6,4,2,
		1,2,3,2,1
	}),
	{ 0,0,0,0 }, 5, 5, 2, 2) };


	std::unique_ptr<EDConvolution> sobelX{ EDConvolution::CreateCustom(
	{
		1, 2, 1,
		0, 0, 0,
		-1, -2, 1,
	},
	{ 0,0,0,0 }, 3, 3, 1, 1) };

	std::unique_ptr<EDConvolution> sobelY{ EDConvolution::CreateCustom(
	{
		-1 , 0, 1,
		-2 , 0, 2,
		-1 , 0, 1,
	},
	{ 0,0,0,0 }, 3, 3, 1, 1) };

	std::unique_ptr<EDConvolution> average{ EDConvolution::CreateCustom(
	EDConvolution::Normalize({
		1,1,1,
		1,1,1,
		1,1,1,
	}),
	{ 0,0,0,0 }, 3, 3, 1, 1) };

	std::unique_ptr<EDConvolution> laplacian{ EDConvolution::CreateCustom(
	{
		-1,-1,-1,
		-1,8,-1,
		-1,-1,-1,
	},
	{ 0,0,0,0 }, 3, 3, 1, 1) };


	composite->push_back(bw);
	composite->push_back(bw);
	
	{
		//std::unique_ptr<EDImage> bwImg{bw->ApplyConvolution(*img)};
		//Save(bwImg.get(), "bw.png");

		//std::unique_ptr<EDImage> srcDest{ bw->ApplyConvolution(*img, img) };
		//Save(srcDest.get(), "srcDest.png");


		//std::unique_ptr<EDImage> compositeImg{ composite->ApplyConvolution(*img) };
		//Save(compositeImg.get(), "composite.png");


		std::unique_ptr<EDImage> blurImg{ blur->ApplyConvolution(*img) };
		Save(blurImg.get(), "blurImg.png");

		std::unique_ptr<EDImage> blurPowerImg { blurPower->ApplyConvolution(*img) };
		Save(blurPowerImg.get(), "blurPowerImg.png");

		std::unique_ptr<EDImage> sobelXImg{ sobelX->ApplyConvolution(*img) };
		Save(sobelXImg.get(), "sobelXImg.png");


		std::unique_ptr<EDImage> sobelYImg{ sobelY->ApplyConvolution(*img) };
		Save(sobelYImg.get(), "sobelYImg.png");

		std::unique_ptr<EDImage> averageImg{ average->ApplyConvolution(*img) };
		Save(averageImg.get(), "averageImg.png");

		std::unique_ptr<EDImage> laplacianImg{ laplacian->ApplyConvolution(*img) };
		Save(laplacianImg.get(), "laplacianImg.png");
		
	/*

		std::unique_ptr<EDImage> bwbwImage{ bw->ApplyConvolution(*bwImg) };

		Save(bwbwImage.get(), "bwbw.png");

	*/



		//std::unique_ptr<EDConvolution> copyPassConv{ EDConvolution::CreateCopyPass() };
		//std::unique_ptr<EDImage> copyPass{ copyPassConv->ApplyConvolution(*img) };

		//Save(copyPass.get(), "copyPass.png");
}


	// (optional) set browser properties
	fileDialog.SetTitle("title");
	fileDialog.SetTypeFilters({ ".jpg", ".png", ".jpeg" });

}

Application::~Application() {
	delete img;
	delete composite;
	delete bw;
}

void Application::Save(EDImage* img, const std::string& path)
{
	if (EDImage::TrySave(img, path))
	{
		std::cout << "success " << path << " save \n";
	}
	else
	{
		std::cout << "failed " << path << " save \n";
	}
}

void Application::ImGui()
{
	ImGui::Begin("Convolution Editor");

	if (ImGui::InputInt("Convolution Height", &heightConv))
	{
		heightConv = clamp(7, 1, heightConv);
	}

	if (ImGui::InputInt("Convolution Width", &widthConv))
	{
		widthConv = clamp(7, 1, widthConv);
	}

	if (ImGui::InputInt("Pivot X", &pivotX))
	{
		pivotX = clamp(widthConv-1, 0, pivotX);
	}

	if (ImGui::InputInt("Pivot Y", &pivotY))
	{
		pivotY = clamp(heightConv - 1, 1, pivotY);
	}



	ImGui::Text("Color button with Picker:");
	ImGui::SameLine(); HelpMarker("With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\nWith the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only be used for the tooltip and picker popup.");


	static std::vector<const char *> names = {
		"0,0",
		"1,0",
		"2,0",
		"3,0",
		"4,0",
		"5,0",
		"6,0",
		"0,1",
		"1,1",
		"2,1",
		"3,1",
		"4,1",
		"5,1",
		"6,1",
		"0,2",
		"1,2",
		"2,2",
		"3,2",
		"4,2",
		"5,2",
		"6,2",
		"0,3",
		"1,3",
		"2,3",
		"3,3",
		"4,3",
		"5,3",
		"6,3",
		"0,4",
		"1,4",
		"2,4",
		"3,4",
		"4,4",
		"5,4",
		"6,4",
		"0,5",
		"1,5",
		"2,5",
		"3,5",
		"4,5",
		"5,5",
		"6,5",
		"0,6",
		"1,6",
		"2,6",
		"3,6",
		"4,6",
		"5,6",
		"6,6",
	};

	for (size_t yy = 0, nn = 0; yy < heightConv; yy++)
	{
		for (size_t xx = 0; xx < widthConv; xx++, nn++)
		{
			ImGui::ColorEdit4(names[nn], (float*)&color[yy][xx], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}

	if (texOGImg)
	{
		//ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
	}

	if (ImGui::Button("Save Image"))
	{
		ImGui::SameLine();
		//ImGui::InputText("imgFile")
	}

	if (ImGui::Button("Load Image"))
	{
		fileDialog.Open();
	}
		fileDialog.Display();


	if (fileDialog.HasSelected())
	{
		std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
		
		fileDialog.Close();
		fileDialog.ClearSelected();
	}


	ImGui::End();

}

void Application::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}