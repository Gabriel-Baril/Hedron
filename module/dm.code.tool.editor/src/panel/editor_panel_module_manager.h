#pragma once

#include "core/core.h"
#include "core/core_filesystem.h"
#include "core/stl/vector.h"

#include "pugixml/pugixml.hpp"

#include "editor_panel.h"

namespace dm
{
	enum class TestExpressionType
	{
		Unknown,
		Check,
		Require
	};

	struct ModuleInfo
	{
		string name;
		u64 version;
		string author;
		string source;
		string semantic;
		string kind;
	};

	struct TestContext
	{
		string testExecutableName;
		u32 rngSeed = 0;
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
		string originalExpression;
		string expandedExpression;
	};

	struct SectionResult
	{
		string name;
		std::filesystem::path filename;
		u32 line;
		vector<ExpressionResult> expressionResults;
		OverallResults overallResults;
	};

	struct TestCaseResult
	{
		string name;
		string tags;
		std::filesystem::path filename;
		u32 line;
		vector<SectionResult> sectionResults{};
		vector<ExpressionResult> expressionResults{};
		OverallResult overallResult{};
	};

	struct TestResult
	{
		TestContext context{};
		vector<TestCaseResult> testCaseResults{};
		OverallResults overallResultsExpressions{};
		OverallResults overallResultsCases{};
	};

	class ModuleManagerPanel : public IEditorPanel
	{
	public:
		ModuleManagerPanel()
				: IEditorPanel{"module manager"}
		{
		}

		void parse_overall_result_node(const pugi::xml_node &resultNode, OverallResult &overallResult);
		void parse_overall_results_node(const pugi::xml_node &resultNode, OverallResults &overallResults);
		void parse_section_node(const pugi::xml_node &sectionNode, SectionResult &sectionResult);
		void parse_expression_node(const pugi::xml_node &expressionNode, ExpressionResult &expressionResult);
		void parse_test_case_node(const pugi::xml_node &testCase, TestCaseResult &testCaseResult);
		void parse_root_node(const pugi::xml_node &root, TestResult &out);
		void load_test_result_from_memory(const string &buffer, TestResult &testResult);
		void display_test_node(const ExpressionResult &expression, ImGuiTreeNodeFlags treeNodeFlags);
		void display_test_node(const SectionResult &section, ImGuiTreeNodeFlags treeNodeFlags);
		void display_test_node(const TestCaseResult &testCase, ImGuiTreeNodeFlags treeNodeFlags);
		void display_test_node(const TestResult &result, ImGuiTreeNodeFlags treeNodeFlags);
		void display_test_node(const vector<TestResult> &results, ImGuiTreeNodeFlags treeNodeFlags);
		void colored_text_if_valid(bool condition, ImVec4 color, int value);
		void set_row_color(bool condition);

		void on_update(f32 dt);

	private:
		std::thread m_WaitThread;
		vector<TestResult> m_TestResults;
		vector<ModuleInfo> m_ModuleInfo;
		bool m_RunningTests = false;
		bool m_RanTestsAtLeastOneTime = false;
	};
}
