#include "editor_panel_module_manager.h"

#include "core/core_filesystem.h"
#include "core/stl/vector.h"

#include "tiny-process-library/process.hpp"

#include "fmt/core.h"

#include "core/config/config.h"


namespace hdn
{
	inline static constexpr const char* ROOT_NODE_NAME = "Catch2TestRun";
	inline static constexpr const char* TEST_CASE_NODE_NAME = "TestCase";
	inline static constexpr const char* SECTION_NODE_NAME = "Section";
	inline static constexpr const char* EXPRESSION_NODE_NAME = "Expression";
	inline static constexpr const char* ORIGINAL_NODE_NAME = "Original";
	inline static constexpr const char* EXPANDED_NODE_NAME = "Expanded";
	inline static constexpr const char* BENCHMARK_NODE_NAME = "BenchmarkResults";
	inline static constexpr const char* OVERALL_RESULTS_NODE_NAME = "OverallResults";
	inline static constexpr const char* OVERALL_RESULTS_CASES_NODE_NAME = "OverallResultsCases";
	inline static constexpr const char* OVERALL_RESULT_NODE_NAME = "OverallResult";

	void ModuleManagerPanel::parse_overall_result_node(const pugi::xml_node& resultNode, OverallResult& overallResult)
	{
		for (const auto& attribute : resultNode.attributes())
		{
			if (str_equals(attribute.name(), "success"))
			{
				overallResult.success = attribute.as_bool();
			}
			else if (str_equals(attribute.name(), "durationInSeconds"))
			{
				overallResult.durationInSeconds = attribute.as_double();
			}
		}
	}
	void ModuleManagerPanel::parse_overall_results_node(const pugi::xml_node& resultNode, OverallResults& overallResults)
	{
		for (const auto& attribute : resultNode.attributes())
		{
			if (str_equals(attribute.name(), "successes"))
			{
				overallResults.successes = attribute.as_uint();
			}
			else if (str_equals(attribute.name(), "failures"))
			{
				overallResults.failures = attribute.as_uint();
			}
			else if (str_equals(attribute.name(), "expectedFailures"))
			{
				overallResults.expectedFailures = attribute.as_uint();
			}
		}
	}

	void ModuleManagerPanel::parse_section_node(const pugi::xml_node& sectionNode, SectionResult& sectionResult)
	{
		for (const auto& attribute : sectionNode.attributes())
		{
			if (str_equals(attribute.name(), "name"))
			{
				sectionResult.name = attribute.as_string();
			}
			else if (str_equals(attribute.name(), "filename"))
			{
				sectionResult.filename = attribute.as_string();
			}
			else if (str_equals(attribute.name(), "line"))
			{
				sectionResult.line = attribute.as_uint();
			}
		}

		for (pugi::xml_node node = sectionNode.first_child(); node; node = node.next_sibling())
		{
			if (str_equals(node.name(), EXPRESSION_NODE_NAME))
			{
				sectionResult.expressionResults.emplace_back(ExpressionResult{});
				parse_expression_node(node, sectionResult.expressionResults.back());
			}
			else if (str_equals(node.name(), OVERALL_RESULTS_NODE_NAME))
			{
				parse_overall_results_node(node, sectionResult.overallResults);
			}
		}
	}
	void ModuleManagerPanel::parse_expression_node(const pugi::xml_node& expressionNode, ExpressionResult& expressionResult)
	{
		for (const auto& attribute : expressionNode.attributes())
		{
			if (str_equals(attribute.name(), "success"))
			{
				expressionResult.success = attribute.as_bool();
			}
			else if (str_equals(attribute.name(), "type"))
			{
				const char* typeStr = attribute.as_string();
				if (str_equals(typeStr, "REQUIRE"))
				{
					expressionResult.type = TestExpressionType::Require;
				}
				else if (str_equals(typeStr, "CHECK"))
				{
					expressionResult.type = TestExpressionType::Check;
				}
			}
			else if (str_equals(attribute.name(), "filename"))
			{
				expressionResult.filename = attribute.as_string();
			}
			else if (str_equals(attribute.name(), "line"))
			{
				expressionResult.line = attribute.as_uint();
			}
		}

		for (pugi::xml_node node = expressionNode.first_child(); node; node = node.next_sibling())
		{
			if (str_equals(node.name(), ORIGINAL_NODE_NAME))
			{
				expressionResult.originalExpression = node.text().as_string();
			}
			else if (str_equals(node.name(), EXPANDED_NODE_NAME))
			{
				expressionResult.expandedExpression = node.text().as_string();
			}

		}
	}

	void ModuleManagerPanel::parse_test_case_node(const pugi::xml_node& testCase, TestCaseResult& testCaseResult)
	{
		HASSERT(str_equals(testCase.name(), TEST_CASE_NODE_NAME), "Invalid Test Case Node");

		for (const auto& attribute : testCase.attributes())
		{
			if (str_equals(attribute.name(), "name"))
			{
				testCaseResult.name = attribute.as_string();
			}
			else if (str_equals(attribute.name(), "tags"))
			{
				testCaseResult.tags = attribute.as_string();
			}
			else if (str_equals(attribute.name(), "filename"))
			{
				testCaseResult.filename = attribute.as_string();
			}
			else if (str_equals(attribute.name(), "line"))
			{
				testCaseResult.line = attribute.as_uint();
			}
		}

		for (pugi::xml_node node = testCase.first_child(); node; node = node.next_sibling())
		{
			if (str_equals(node.name(), SECTION_NODE_NAME))
			{
				testCaseResult.sectionResults.emplace_back(SectionResult{});
				parse_section_node(node, testCaseResult.sectionResults.back());
			}
			else if (str_equals(node.name(), EXPRESSION_NODE_NAME))
			{
				testCaseResult.expressionResults.emplace_back(ExpressionResult{});
				parse_expression_node(node, testCaseResult.expressionResults.back());
			}
			else if (str_equals(node.name(), BENCHMARK_NODE_NAME))
			{
				// TODO: Parse Benchmark Results
			}
			else if (str_equals(node.name(), OVERALL_RESULT_NODE_NAME))
			{
				parse_overall_result_node(node, testCaseResult.overallResult);
			}
		}
	}

	void ModuleManagerPanel::parse_root_node(const pugi::xml_node& root, TestResult& out)
	{
		HASSERT(str_equals(root.name(), ROOT_NODE_NAME), "Invalid Root Node");

		out.context.testExecutableName = root.attribute("name").as_string();
		out.context.rngSeed = root.attribute("rng-seed").as_uint();

		for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling())
		{
			if (str_equals(node.name(), TEST_CASE_NODE_NAME))
			{
				out.testCaseResults.emplace_back(TestCaseResult{});
				parse_test_case_node(node, out.testCaseResults.back());
			}
			else if (str_equals(node.name(), OVERALL_RESULTS_NODE_NAME))
			{
				parse_overall_results_node(node, out.overallResultsExpressions);
			}
			else if (str_equals(node.name(), OVERALL_RESULTS_CASES_NODE_NAME))
			{
				parse_overall_results_node(node, out.overallResultsCases);
			}
		}
	}

	void ModuleManagerPanel::load_test_result_from_memory(const string& buffer, TestResult& testResult)
	{
		pugi::xml_document doc; // Already contains the root node (Catch2TestRun)
		pugi::xml_parse_result result = doc.load_buffer(buffer.c_str(), buffer.size());
		if (!result)
		{
			return;
		}
		parse_root_node(doc.first_child(), testResult);
	}

	void ModuleManagerPanel::display_test_node(const ExpressionResult& expression, ImGuiTreeNodeFlags treeNodeFlags)
	{
		ImGui::TableNextRow();
		set_row_color(expression.success);
		string expandedExpression = trim(expression.expandedExpression);
		ImGui::TableNextColumn();
		ImGui::TreeNodeEx(expandedExpression.c_str(), treeNodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
		ImGui::TableNextColumn();
		ImGui::Text("Expression");
		ImGui::TableNextColumn();
		colored_text_if_valid(expression.success, ImVec4(0.0f, 1.0f, 0.0f, 1.0f), expression.success);
		ImGui::TableNextColumn();
		colored_text_if_valid(!expression.success, ImVec4(1.0f, 0.0f, 0.0f, 1.0f), !expression.success);
		ImGui::TableNextColumn();
		ImGui::Text("%s", expression.filename.string().c_str());
		ImGui::TableNextColumn();
		ImGui::Text("%i", expression.line);
	}

	void ModuleManagerPanel::display_test_node(const SectionResult& section, ImGuiTreeNodeFlags treeNodeFlags)
	{
		ImGui::TableNextRow();
		set_row_color(section.overallResults.failures <= 0);

		ImGui::TableNextColumn();
		bool open = ImGui::TreeNodeEx(section.name.c_str(), treeNodeFlags);
		ImGui::TableNextColumn();
		ImGui::Text("Section");
		ImGui::TableNextColumn();
		colored_text_if_valid(section.overallResults.successes > 0, ImVec4(0.0f, 1.0f, 0.0f, 1.0f), section.overallResults.successes);
		ImGui::TableNextColumn();
		colored_text_if_valid(section.overallResults.failures > 0, ImVec4(1.0f, 0.0f, 0.0f, 1.0f), section.overallResults.failures);
		ImGui::TableNextColumn();
		ImGui::Text("%s", section.filename.string().c_str());
		ImGui::TableNextColumn();
		ImGui::Text("%i", section.line);

		if (open)
		{
			for (const ExpressionResult& expression : section.expressionResults)
			{
				display_test_node(expression, treeNodeFlags);
			}
			ImGui::TreePop();
		}
	}

	void ModuleManagerPanel::display_test_node(const TestCaseResult& testCase, ImGuiTreeNodeFlags treeNodeFlags)
	{
		ImGui::TableNextRow();
		set_row_color(testCase.overallResult.success);

		ImGui::TableNextColumn();
		bool open = ImGui::TreeNodeEx(testCase.name.c_str(), treeNodeFlags);
		ImGui::TableNextColumn();
		ImGui::Text("Test Case");
		ImGui::TableNextColumn();
		colored_text_if_valid(testCase.overallResult.success, ImVec4(0.0f, 1.0f, 0.0f, 1.0f), testCase.overallResult.success);
		ImGui::TableNextColumn();
		colored_text_if_valid(!testCase.overallResult.success, ImVec4(1.0f, 0.0f, 0.0f, 1.0f), !testCase.overallResult.success);
		ImGui::TableNextColumn();
		ImGui::Text("%s", testCase.filename.string().c_str());
		ImGui::TableNextColumn();
		ImGui::Text("%i", testCase.line);

		if (open)
		{
			for (const SectionResult& section : testCase.sectionResults)
			{
				display_test_node(section, treeNodeFlags);
			}
			for (const ExpressionResult& expression : testCase.expressionResults)
			{
				display_test_node(expression, treeNodeFlags);
			}
			ImGui::TreePop();
		}
	}

	void ModuleManagerPanel::display_test_node(const TestResult& result, ImGuiTreeNodeFlags treeNodeFlags)
	{
		ImGui::TableNextRow();
		set_row_color(result.overallResultsCases.failures <= 0);

		ImGui::TableNextColumn();
		bool open = ImGui::TreeNodeEx(result.context.testExecutableName.c_str(), treeNodeFlags);
		ImGui::TableNextColumn();
		ImGui::Text("Module");
		ImGui::TableNextColumn();
		colored_text_if_valid(result.overallResultsCases.successes > 0, ImVec4(0.0f, 1.0f, 0.0f, 1.0f), result.overallResultsCases.successes);
		ImGui::TableNextColumn();
		colored_text_if_valid(result.overallResultsCases.failures > 0, ImVec4(1.0f, 0.0f, 0.0f, 1.0f), result.overallResultsCases.failures);
		ImGui::TableNextColumn();
		ImGui::TextDisabled("--");
		ImGui::TableNextColumn();
		ImGui::TextDisabled("--");

		if (open)
		{
			for (const TestCaseResult& testCase : result.testCaseResults)
			{
				display_test_node(testCase, treeNodeFlags);
			}
			ImGui::TreePop();
		}
	}

	void ModuleManagerPanel::display_test_node(const vector<TestResult>& results, ImGuiTreeNodeFlags treeNodeFlags)
	{
		for (const TestResult& result : results)
		{
			display_test_node(result, treeNodeFlags);
		}
	}

	void ModuleManagerPanel::colored_text_if_valid(bool condition, ImVec4 color, int value)
	{
		if (condition)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::Text("%i", value);
			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::Text("%i", value);
		}
	}

	void ModuleManagerPanel::set_row_color(bool condition)
	{
		if (condition)
		{
			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(0, 255, 0, 35));
		}
		else
		{
			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, IM_COL32(255, 0, 0, 35));
		}
	}

	void ModuleManagerPanel::on_update(f32 dt)
	{
		if (ImGui::Button("Run All Tests"))
		{
			// 1. Run build_test_projects.py
			if (!m_RunningTests)
			{
				HWARN("build_test_projects.py started...");

				// TODO: Refactor, proper multhreading management
				m_WaitThread = std::thread([this]() {
					this->m_RunningTests = true;

					const string testSolutionPath = Configuration::get().get_root_config_variable(CONFIG_SECTION_TEST, CONFIG_KEY_TEST_SOLUTION_PATH, "");
					const string buildTestProjectScriptPath = Configuration::get().get_root_config_variable(CONFIG_SECTION_TEST, CONFIG_KEY_BUILD_TEST_PROJECT_SCRIPT_PATH, "");
					const string command = fmt::format("python {0}", buildTestProjectScriptPath);

					// 2. Compile the list of test projects concurently
					TinyProcessLib::Process process(
						command,
						testSolutionPath.c_str(),
						[](const char* /*output*/, size_t /*n*/) {
							// Do nothing for stdout
						},
						[](const char* /*error*/, size_t /*n*/) {
							// Do nothing for stderr
						}
					);
					int exitStatus = process.get_exit_status();
					if (exitStatus != 0)
					{
						HERR("Process terminated with errors");
					}
					HWARN("----------- Finished Running build_test_projects.py -----------");

					// 3. Run the test project executable with the right arguments (--success --durations yes --verbosity high --allow-running-no-tests --reporter xml > test_result.xml)
					const string executableListFilePath = Configuration::get().get_root_config_variable(CONFIG_SECTION_TEST, CONFIG_KEY_EXECUTABLE_LIST_PATH, "");
					std::ifstream inputFile(executableListFilePath);
					if (!inputFile.is_open())
					{
						HERR("Error: Could not open the file '{0}'", executableListFilePath.c_str());
					}

					string line;
					vector<string> executableCommands;
					executableCommands.reserve(10);
					while (std::getline(inputFile, line))
					{
						string executableCommand = fmt::format("{0} --success --durations yes --verbosity high --allow-running-no-tests --reporter xml", line);
						executableCommands.push_back(executableCommand);
						HINFO("Executable -> '{0}' '{1}'", line.c_str(), executableCommand.c_str());
					}

					// TODO: Make this concurrent
					vector<string> testResultsStrs;
					testResultsStrs.reserve(10);
					for (const auto& testCommand : executableCommands)
					{
						HINFO("Running -> '{0}'", testCommand);
						testResultsStrs.emplace_back(string{});
						string& testResultXML = testResultsStrs.back();
						TinyProcessLib::Process testExecutableProcess(
							testCommand,
							"",
							[&testResultXML](const char* output, size_t n) {
								// 4. Collect all the test generated outputs
								testResultXML.append(output, n);
							},
							[](const char* error, size_t n) {
								for (int i = 0; i < n; i++)
								{
									HERR("{0}", error[i]);
								}
							}
						);
						int status = process.get_exit_status();
						if (status != 0)
						{
							HERR("Process terminated with errors");
						}
					}

					HWARN("Tests Results Finished!");

					m_TestResults.clear();
					for (const auto& testResultsStr : testResultsStrs)
					{
						TestResult result;
						// 5. Parse the generated test output
						this->load_test_result_from_memory(testResultsStr, result);
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

		const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
		const ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

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

				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAllColumns;
				display_test_node(m_TestResults, treeNodeFlags);

				ImGui::EndTable();
			}
		}

		ImGui::Separator();

		if (ImGui::Button("Get Module Info"))
		{
			m_ModuleInfo.clear();
			const string rootModuleFolder = Configuration::get().get_root_config_variable(CONFIG_SECTION_PATH, CONFIG_KEY_MODULE_FOLDER_PATH, "");
			const vector<fspath> moduleFolders = filesystem_walk(rootModuleFolder);
			for (const auto& modulefolder : moduleFolders)
			{
				fspath moduleFilename = ".module";
				fspath moduleConfigFile = modulefolder / moduleFilename;

				if (!filesystem_exists(moduleConfigFile))
				{
					continue;
				}

				INIReader reader(moduleConfigFile.string());
				ModuleInfo info;
				info.name = filesystem_filename(filesystem_parent(moduleConfigFile)).string();
				info.version = reader.GetUnsigned64("module", "Version", 0);
				info.author = reader.GetString("module", "Author", "unknown");
				info.source = reader.GetString("module", "Source", "unknown");
				info.semantic = reader.GetString("module", "Semantic", "unknown");
				info.kind = reader.GetString("module", "Kind", "unknown");
				m_ModuleInfo.push_back(info);
			}
		}

		if (m_ModuleInfo.size() > 0 && ImGui::BeginTable("module", 6, flags))
		{
			// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
			ImGui::TableSetupColumn("Module", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("Version", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			ImGui::TableSetupColumn("Author", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			ImGui::TableSetupColumn("Source", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			ImGui::TableSetupColumn("Semantic", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			ImGui::TableSetupColumn("Kind", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
			ImGui::TableHeadersRow();

			for (const auto& moduleInfo : m_ModuleInfo)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("%s", moduleInfo.name.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%i", moduleInfo.version);
				ImGui::TableNextColumn();
				ImGui::Text("%s", moduleInfo.author.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", moduleInfo.source.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", moduleInfo.semantic.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", moduleInfo.kind.c_str());
			}

			ImGui::EndTable();
		}
	}
}