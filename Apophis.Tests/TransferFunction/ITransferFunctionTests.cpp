#include "stdafx.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis::TransferFunction;

namespace ApophisTests { namespace TransferFunction	{

	TEST_CLASS(ITransferFunctionTests)
	{
	public:
		TEST_METHOD(Get_Relu)
		{
			auto& function = ITransferFunction::Get("relu");
			Assert::IsTrue(&function == &Relu::Default());
		}

		TEST_METHOD(Get_Sigmoid)
		{
			auto& function = ITransferFunction::Get("sigmoid");
			Assert::IsTrue(&function == &Sigmoid::Default());
		}
	};

}}