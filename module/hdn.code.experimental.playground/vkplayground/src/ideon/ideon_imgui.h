#pragma once

#include "imgui.h"
#include "pugixml/pugixml.hpp"
#include "tiny-process-library/process.hpp"

#include <filesystem>
#include <vector>
#include <iostream>

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

		void LoadTestResultFile(const std::string& path, TestResult& testResult)
		{
			pugi::xml_document doc; // Already contains the root node (Catch2TestRun)
			pugi::xml_parse_result result = doc.load_file(path.c_str());
			if (!result)
			{
				return;
			}
			ParseRootNode(doc.first_child(), testResult);
			HINFO("File parsed!");
		}

		void LoadTestResultFromMemory(const std::string& buffer, TestResult& testResult)
		{
			pugi::xml_document doc; // Already contains the root node (Catch2TestRun)
			pugi::xml_parse_result result = doc.load_buffer(buffer.c_str(), buffer.size());
			if (!result)
			{
				return;
			}
			ParseRootNode(doc.first_child(), testResult);
		}


		void DisplayTestNode(const ExpressionResult& expression, ImGuiTreeNodeFlags treeNodeFlags)
		{
			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			ImGui::TreeNodeEx(expression.originalExpression.c_str(), treeNodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("Expression");
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", expression.success);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", !expression.success);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%s", expression.filename.c_str());
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", expression.line);
		}

		void DisplayTestNode(const SectionResult& section, ImGuiTreeNodeFlags treeNodeFlags)
		{
			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			bool open = ImGui::TreeNodeEx(section.name.c_str(), treeNodeFlags);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("Section");
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", section.overallResults.successes);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", section.overallResults.failures);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%s", section.filename.c_str());
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", section.line);

			if (open)
			{
				for (const ExpressionResult& expression : section.expressionResults)
				{
					DisplayTestNode(expression, treeNodeFlags);
				}
				ImGui::TreePop();
			}
		}

		void DisplayTestNode(const TestCaseResult& testCase, ImGuiTreeNodeFlags treeNodeFlags)
		{
			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			bool open = ImGui::TreeNodeEx(testCase.name.c_str(), treeNodeFlags);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("Test Case");
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", testCase.overallResult.success);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", !testCase.overallResult.success);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%s", testCase.filename.c_str());
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", testCase.line);

			if (open)
			{
				for (const SectionResult& section : testCase.sectionResults)
				{
					DisplayTestNode(section, treeNodeFlags);
				}
				for (const ExpressionResult& expression : testCase.expressionResults)
				{
					DisplayTestNode(expression, treeNodeFlags);
				}
				ImGui::TreePop();
			}
		}

		void DisplayTestNode(const TestResult& result, ImGuiTreeNodeFlags treeNodeFlags)
		{
			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			bool open = ImGui::TreeNodeEx(result.context.testExecutableName.c_str(), treeNodeFlags);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("Module");
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", result.overallResultsCases.successes);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("%i", result.overallResultsCases.failures);
			ImGui::TableNextColumn();
			ImGui::TextDisabled("--");
			ImGui::TableNextColumn();
			ImGui::TextDisabled("--");

			if (open)
			{
				for (const TestCaseResult& testCase : result.testCaseResults)
				{
					DisplayTestNode(testCase, treeNodeFlags);
				}
				ImGui::TreePop();
			}
		}

		void DisplayTestNode(const std::vector<TestResult>& results, ImGuiTreeNodeFlags treeNodeFlags)
		{
			for (const TestResult& result : results)
			{
				DisplayTestNode(result, treeNodeFlags);
			}
		}

		void Draw()
		{
			ImGui::Begin("IdeonUI");
			if (ImGui::Button("Run All Tests"))
			{
				// 1. Run build_test_projects.py
				if (!m_RunningTests)
				{
					HWARN("build_test_projects.py started...");

					// TODO: Refactor, proper multhreading management
					m_WaitThread = std::thread([this]() {
						this->m_RunningTests = true;
						std::string command = "python D:/_DEV/hedron/module/hdn.tests/build_test_projects.py"; // TODO: Remove hardcoded path and read it from hedron.ini file

						// 2. Compile the list of test projects concurently
						TinyProcessLib::Process process(
							command,
							"D:/_DEV/hedron/module/hdn.tests",
							[](const char* /*output*/, size_t /*n*/) {
								// Do nothing for stdout
							},
							[](const char* /*error*/, size_t /*n*/) {
								// Do nothing for stderr
							}
						);
						int exitStatus = process.get_exit_status();
						HWARN("----------- Finished Running build_test_projects.py -----------");

						// 3. Run the test project executable with the right arguments (--success --durations yes --verbosity high --allow-running-no-tests --reporter xml > test_result.xml)
						std::string filePath = "D:/_DEV/hedron/module/hdn.tests/test_executable_list.txt"; // TODO: Refactor, Proper file loading library
						std::ifstream inputFile(filePath);
						if (!inputFile.is_open())
						{
							HERR("Error: Could not open the file '{0}'", filePath.c_str());
						}

						std::string line;
						std::vector<std::string> outTestResultPaths;
						std::vector<std::string> executableCommands;
						outTestResultPaths.reserve(10);
						executableCommands.reserve(10);
						while (std::getline(inputFile, line))
						{
							std::filesystem::path executableFile = line;
							std::string executableName = executableFile.stem().string();
							std::string outTestResultFilePath = "D:/_DEV/hedron/module/hdn.tests/" + executableName + ".xml";
							// std::string executableCommand = line + " --success --durations yes --verbosity high --allow-running-no-tests --reporter xml > " + outTestResultFilePath; // TODO: Refactor, proper string format library
							std::string executableCommand = line + " --success --durations yes --verbosity high --allow-running-no-tests --reporter xml"; // TODO: Refactor, proper string format library
							executableCommands.push_back(executableCommand);
							outTestResultPaths.push_back(outTestResultFilePath);
							HINFO("Executable -> '{0}' '{1}'", line.c_str(), executableCommand.c_str());
						}

						// TODO: Make this concurrent
						std::vector<std::string> testResultsStrs;
						testResultsStrs.reserve(10);
						for (const auto& testCommand : executableCommands)
						{
							HINFO("Running -> '{0}'", testCommand);
							testResultsStrs.emplace_back(std::string{});
							std::string& testResultXML = testResultsStrs.back();
							TinyProcessLib::Process testExecutableProcess(
								testCommand,
								"",
								[&testResultXML](const char* output, size_t n) {
									// 4. Collect all the test generated outputs
									testResultXML.append(output, n);
								},
								[](const char* error, size_t n) {
									for (int i = 0;i < n;i++)
									{
										std::cout << error[i];
									}
									std::cout << "\n";
								}
							);
							int status = process.get_exit_status();
						}

						HWARN("Tests Results Finished!");

						m_TestResults.clear();
						for (const auto& testResultsStr : testResultsStrs)
						{
							TestResult result;
							// 5. Parse the generated test output
							this->LoadTestResultFromMemory(testResultsStr, result);
							HINFO("RESULT: {0}", result.context.testExecutableName.c_str());
							m_TestResults.emplace_back(result);
						}
						this->m_RunningTests = false;
						m_RanTestsAtLeastOneTime = true;
					});
					m_WaitThread.detach();
				}
				else
				{
					HWARN("Test already running");
				}
			}

			if (m_RunningTests)
			{
				ImGui::Text("Running Tests...");
			}
			else if (!m_RanTestsAtLeastOneTime)
			{
				ImGui::Text("Please Run the Tests");
			}
			else
			{
				HelpMarker("See \"Columns flags\" section to configure how indentation is applied to individual columns.");

				const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
				const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

				const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

				if (ImGui::BeginTable("ideon", 6, flags))
				{
					// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
					ImGui::TableSetupColumn("Module", ImGuiTableColumnFlags_NoHide);
					ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
					ImGui::TableSetupColumn("Success", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
					ImGui::TableSetupColumn("Failed", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
					ImGui::TableSetupColumn("Filename", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
					ImGui::TableSetupColumn("Line", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
					ImGui::TableHeadersRow();

					// Simple storage to output a dummy file-system.
					/*
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
					*/
					const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAllColumns;
					DisplayTestNode(m_TestResults, treeNodeFlags);

					ImGui::EndTable();
				}
			}

			ImGui::End();
		}

	private:
		std::thread m_WaitThread;
		std::vector<TestResult> m_TestResults;
		bool m_RunningTests = false;
		bool m_RanTestsAtLeastOneTime = false;
	};
}