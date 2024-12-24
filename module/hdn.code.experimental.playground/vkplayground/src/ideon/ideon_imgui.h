#pragma once
#include "imgui.h"
#include "pugixml/pugixml.hpp"

#include <filesystem>
#include <vector>

namespace hdn
{
	enum class TestExpressionType
	{
		Unknown,
		Check,
		Require
	};

	struct TestContext
	{
		std::string testExecutableName;
		u32 rngSeed;
	};

	struct OverallResult
	{
		bool success;
		f64 durationInSeconds;
	};

	struct OverallResults
	{
		u32 successes;
		u32 failures;
		u32 expectedFailures;
		// u32 skips;
		// durationInSeconds
	};

	struct ExpressionResult
	{
		bool success;
		TestExpressionType type;
		std::filesystem::path filename;
		u32 line;
		std::string originalExpression;
		std::string expandedExpression;
	};

	struct SectionResult
	{
		std::string name;
		std::filesystem::path filename;
		u32 line;
		std::vector<ExpressionResult> expressionResults;
		OverallResults overallResults;
	};

	struct TestCaseResult
	{
		std::string name;
		std::string tags;
		std::filesystem::path filename;
		u32 line;
		std::vector<SectionResult> sectionResults;
		std::vector<ExpressionResult> expressionResults;
		OverallResult overallResult;
	};

	struct TestResult
	{
		TestContext context;
		std::vector<TestCaseResult> testCaseResults;
		OverallResults overallResultsExpressions;
		OverallResults overallResultsCases;
	};

	class IdeonImgui
	{
	public:
		static constexpr const char* ROOT_NODE_NAME = "Catch2TestRun";
		static constexpr const char* TEST_CASE_NODE_NAME = "TestCase";
		static constexpr const char* SECTION_NODE_NAME = "Section";
		static constexpr const char* EXPRESSION_NODE_NAME = "Expression";
		static constexpr const char* ORIGINAL_NODE_NAME = "Original";
		static constexpr const char* EXPANDED_NODE_NAME = "Expanded";
		static constexpr const char* BENCHMARK_NODE_NAME = "BenchmarkResults";
		static constexpr const char* OVERALL_RESULTS_NODE_NAME = "OverallResults";
		static constexpr const char* OVERALL_RESULTS_CASES_NODE_NAME = "OverallResultsCases";
		static constexpr const char* OVERALL_RESULT_NODE_NAME = "OverallResult";

		static void HelpMarker(const char* desc)
		{
			ImGui::TextDisabled("(?)");
			if (ImGui::BeginItemTooltip())
			{
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(desc);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}

		void ParseOverallResultNode(const pugi::xml_node& resultNode, OverallResult& overallResult)
		{
			for (const auto& attribute : resultNode.attributes())
			{
				if (Str_Equals(attribute.name(), "success"))
				{
					overallResult.success = attribute.as_bool();
				}
				else if (Str_Equals(attribute.name(), "durationInSeconds"))
				{
					overallResult.durationInSeconds = attribute.as_double();
				}
			}
		}

		void ParseOverallResultsNode(const pugi::xml_node& resultNode, OverallResults& overallResults)
		{
			for (const auto& attribute : resultNode.attributes())
			{
				if (Str_Equals(attribute.name(), "successes"))
				{
					overallResults.successes = attribute.as_uint();
				}
				else if (Str_Equals(attribute.name(), "failures"))
				{
					overallResults.failures = attribute.as_uint();
				}
				else if (Str_Equals(attribute.name(), "expectedFailures"))
				{
					overallResults.expectedFailures = attribute.as_uint();
				}
			}
		}

		void ParseSectionNode(const pugi::xml_node& sectionNode, SectionResult& sectionResult)
		{
			for (const auto& attribute : sectionNode.attributes())
			{
				if (Str_Equals(attribute.name(), "name"))
				{
					sectionResult.name = attribute.as_string();
				}
				else if (Str_Equals(attribute.name(), "filename"))
				{
					sectionResult.filename = attribute.as_string();
				}
				else if (Str_Equals(attribute.name(), "line"))
				{
					sectionResult.line = attribute.as_uint();
				}
			}

			for (pugi::xml_node node = sectionNode.first_child(); node; node = node.next_sibling())
			{
				if (Str_Equals(node.name(), EXPRESSION_NODE_NAME))
				{
					sectionResult.expressionResults.emplace_back(ExpressionResult{});
					ParseExpressionNode(node, sectionResult.expressionResults.back());
				}
				else if (Str_Equals(node.name(), OVERALL_RESULTS_NODE_NAME))
				{
					ParseOverallResultsNode(node, sectionResult.overallResults);
				}
			}
		}

		void ParseExpressionNode(const pugi::xml_node& expressionNode, ExpressionResult& expressionResult)
		{
			for (const auto& attribute : expressionNode.attributes())
			{
				if (Str_Equals(attribute.name(), "success"))
				{
					expressionResult.success = attribute.as_bool();
				}
				else if (Str_Equals(attribute.name(), "type"))
				{
					const char* typeStr = attribute.as_string();
					if (Str_Equals(typeStr, "REQUIRE"))
					{
						expressionResult.type = TestExpressionType::Require;
					}
					else if (Str_Equals(typeStr, "CHECK"))
					{
						expressionResult.type = TestExpressionType::Check;
					}
				}
				else if (Str_Equals(attribute.name(), "filename"))
				{
					expressionResult.filename = attribute.as_string();
				}
				else if (Str_Equals(attribute.name(), "line"))
				{
					expressionResult.line = attribute.as_uint();
				}
			}

			for (pugi::xml_node node = expressionNode.first_child(); node; node = node.next_sibling())
			{
				if (Str_Equals(node.name(), ORIGINAL_NODE_NAME))
				{
					expressionResult.originalExpression = node.text().as_string();
				}
				else if (Str_Equals(node.name(), EXPANDED_NODE_NAME))
				{
					expressionResult.expandedExpression = node.text().as_string();
				}
				
			}
		}

		void ParseTestCaseNode(const pugi::xml_node& testCase, TestCaseResult& testCaseResult)
		{
			HASSERT(Str_Equals(testCase.name(), TEST_CASE_NODE_NAME), "Invalid Test Case Node");

			for (const auto& attribute : testCase.attributes())
			{
				if (Str_Equals(attribute.name(), "name"))
				{
					testCaseResult.name = attribute.as_string();
				}
				else if (Str_Equals(attribute.name(), "tags"))
				{
					testCaseResult.tags = attribute.as_string();
				}
				else if (Str_Equals(attribute.name(), "filename"))
				{
					testCaseResult.filename = attribute.as_string();
				}
				else if (Str_Equals(attribute.name(), "line"))
				{
					testCaseResult.line = attribute.as_uint();
				}
			}

			for (pugi::xml_node node = testCase.first_child(); node; node = node.next_sibling())
			{
				if (Str_Equals(node.name(), SECTION_NODE_NAME))
				{
					testCaseResult.sectionResults.emplace_back(SectionResult{});
					ParseSectionNode(node, testCaseResult.sectionResults.back());
				}
				else if (Str_Equals(node.name(), EXPRESSION_NODE_NAME))
				{
					testCaseResult.expressionResults.emplace_back(ExpressionResult{});
					ParseExpressionNode(node, testCaseResult.expressionResults.back());
				}
				else if (Str_Equals(node.name(), BENCHMARK_NODE_NAME))
				{
					// TODO: Parse Benchmark Results
				}
				else if (Str_Equals(node.name(), OVERALL_RESULT_NODE_NAME))
				{
					ParseOverallResultNode(node, testCaseResult.overallResult);
				}
			}
		}

		void ParseRootNode(const pugi::xml_node& root, TestResult& out)
		{
			HASSERT(Str_Equals(root.name(), ROOT_NODE_NAME), "Invalid Root Node");

			out.context.testExecutableName = root.attribute("name").as_string();
			out.context.rngSeed = root.attribute("rng-seed").as_uint();

			for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling())
			{
				HINFO(node.name());
				if (Str_Equals(node.name(), TEST_CASE_NODE_NAME))
				{
					out.testCaseResults.emplace_back(TestCaseResult{});
					ParseTestCaseNode(node, out.testCaseResults.back());
				}
				else if (Str_Equals(node.name(), OVERALL_RESULTS_NODE_NAME))
				{
					ParseOverallResultsNode(node, out.overallResultsExpressions);
				}
				else if (Str_Equals(node.name(), OVERALL_RESULTS_CASES_NODE_NAME))
				{
					ParseOverallResultsNode(node, out.overallResultsCases);
				}
			}
		}

		void LoadTestResultFile(const std::string& path)
		{
			pugi::xml_document doc; // Already contains the root node (Catch2TestRun)
			pugi::xml_parse_result result = doc.load_file(path.c_str());
			if (!result)
			{
				return;
			}
			TestResult testResult{};
			ParseRootNode(doc.first_child(), testResult);



			HINFO("File parsed!");
		}

		void Draw()
		{
			const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
			const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

			const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
			const ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;

			ImGui::Begin("IdeonUI");
			HelpMarker("See \"Columns flags\" section to configure how indentation is applied to individual columns.");
			if (ImGui::Button("Run All Tests"))
			{
				// 1. Gather all the test projects
				// 2. Compile the list of test projects concurently
				// 3. Run the test project executable with the right arguments (--success --durations yes --verbosity high --allow-running-no-tests --reporter xml > test_result.xml)
				// 4. Collect all the generated 'test_result.xml' files
				// 5. Parse the generated 'test_result.xml' files
				// 6. Present the parsed result into a UI
			}


			if (ImGui::BeginTable("ideon", 3, flags))
			{
				// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
				ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
				ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
				ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
				ImGui::TableHeadersRow();

				// Simple storage to output a dummy file-system.
				struct MyTreeNode
				{
					const char* Name;
					const char* Type;
					int         Size;
					int         ChildIdx;
					int         ChildCount;
					static void DisplayNode(const MyTreeNode* node, const MyTreeNode* all_nodes)
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						const bool is_folder = (node->ChildCount > 0);
						if (is_folder)
						{
							bool open = ImGui::TreeNodeEx(node->Name, tree_node_flags);
							ImGui::TableNextColumn();
							ImGui::TextDisabled("--");
							ImGui::TableNextColumn();
							ImGui::TextUnformatted(node->Type);
							if (open)
							{
								for (int child_n = 0; child_n < node->ChildCount; child_n++)
									DisplayNode(&all_nodes[node->ChildIdx + child_n], all_nodes);
								ImGui::TreePop();
							}
						}
						else
						{
							ImGui::TreeNodeEx(node->Name, tree_node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
							ImGui::TableNextColumn();
							ImGui::Text("%d", node->Size);
							ImGui::TableNextColumn();
							ImGui::TextUnformatted(node->Type);
						}
					}
				};

				static const MyTreeNode nodes[] =
				{
					{ "Root",                         "Folder",       -1,       1, 3    }, // 0
					{ "Music",                        "Folder",       -1,       4, 2    }, // 1
					{ "Textures",                     "Folder",       -1,       6, 3    }, // 2
					{ "desktop.ini",                  "System file",  1024,    -1,-1    }, // 3
					{ "File1_a.wav",                  "Audio file",   123000,  -1,-1    }, // 4
					{ "File1_b.wav",                  "Audio file",   456000,  -1,-1    }, // 5
					{ "Image001.png",                 "Image file",   203128,  -1,-1    }, // 6
					{ "Copy of Image001.png",         "Image file",   203256,  -1,-1    }, // 7
					{ "Copy of Image001 (Final2).png","Image file",   203512,  -1,-1    }, // 8
				};

				MyTreeNode::DisplayNode(&nodes[0], nodes);

				ImGui::EndTable();
			}
			ImGui::End();
		}

	private:
	};
}