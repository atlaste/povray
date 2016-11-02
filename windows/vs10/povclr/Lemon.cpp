#include "Stdafx.h"
#include "Lemon.h"

namespace povclr
{
	struct TrivialMessenger : public pov::GenericMessenger
	{
	public:
		TrivialMessenger() :
			pov::GenericMessenger(0, ""),
			result(nullptr)
		{}

		const char* result;

	protected:
		virtual void SendMessage(pov::MessageClass mc, pov::WarningLevel level, const char *text,
								 const UCS2 *filename = NULL, POV_LONG line = -1, POV_LONG column = -1, POV_LONG offset = -1) override
		{
			result = text;
		}
	};

	struct TrivialFileHandle : pov::ITextStream
	{
		virtual int getchar()
		{
			return -1;
		}

		virtual void ungetchar(int) {}

		virtual bool eof() const
		{
			return true;
		}

		virtual bool seekg(FilePos) { return true; }

		virtual FilePos tellg() const
		{
			return FilePos();
		}

		virtual bool ReadRaw(unsigned char* buf, size_t size)
		{
			return false;
		}

		virtual const UCS2 *name() const
		{
			static POV_UINT16 zero = 0;
			return &zero;
		}
	};

	void Lemon::Render(Context^ context)
	{
		auto obj = new pov::Lemon();
		obj->base = Base->ToVector();
		obj->apex = Apex->ToVector();
		obj->base_radius = BaseRadius;
		obj->apex_radius = ApexRadius;
		obj->inner_radius = InnerRadius;

		if ((obj->apex_radius < 0) || (obj->base_radius < 0) || (obj->inner_radius < 0))
		{
			context->Error("All radii must be positive");
		}

		// Really...? Ewww...
		TrivialMessenger msg;
		TrivialFileHandle fh;
		auto pos = fh.tellg();
		obj->Compute_Lemon_Data(msg, &fh, pos, 0);
		if (msg.result)
		{
			context->Warning(msg.result);
		}

		obj->Compute_BBox();

		CSGObject::RenderDetail(context, obj);

	}
}