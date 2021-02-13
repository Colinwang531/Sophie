#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "Error.h"
#if (defined HAVE_HELMET) || (defined HAVE_SLEEP) || (defined HAVE_PHONE) || (defined HAVE_FACE)
#include "Decoder/SDK/HKStreamDecoder.h"
#endif//HAVE_HELMET
#ifdef HAVE_AIS
#include "Decoder/AIS/AISDataDecoder.h"
#endif//HAVE_AIS
#include "Decoder/Decoder.h"

namespace framework
{
	namespace multimedia
	{
		Decoder::Decoder() : decoder{ nullptr }
		{}
		Decoder::~Decoder()
		{
			boost::checked_delete(decoder);
		}

		int Decoder::inputData(DataPtr data)
		{
			int e{ data ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
				const framework::data::DataFactory df{ data->getDataFactory() };

				if (framework::data::DataFactory::DATA_FACTORY_HK == df)
				{
#if (defined HAVE_HELMET) || (defined HAVE_SLEEP) || (defined HAVE_PHONE) || (defined HAVE_FACE)
					HKStreamDecoder* dp{ reinterpret_cast<HKStreamDecoder*>(decoder) };

					if (!dp)
					{
						dp = new(std::nothrow) HKStreamDecoder(
							boost::bind(&Decoder::afterDataDecodeCallback, this, _1));
						decoder = dp;
					}

					e = dp ? dp->inputData(data) : eBadNewObject;
#endif//(defined HAVE_HELMET) || (defined HAVE_SLEEP) || (defined HAVE_PHONE) || (defined HAVE_FACE)
				}
#ifdef HAVE_AIS
				if (framework::data::DataFactory::DATA_FACTORY_AIS == df)
				{
					AISDataDecoder* dp{ reinterpret_cast<AISDataDecoder*>(decoder) };

					if (!dp)
					{
						dp = new(std::nothrow) AISDataDecoder(
							boost::bind(&Decoder::afterDataDecodeCallback, this, _1));
						decoder = dp;
					}

					e = dp ? dp->inputData(data) : eBadNewObject;
				}
#endif//HAVE_AIS
				else
				{
					e = eNotSupport;
				}
			}

			return e;
		}

		void Decoder::afterDataDecodeCallback(DataPtr data)
		{
			if (data)
			{
				afterDataDecodeNotification(data);
			}
		}
	}//namespace multimedia
}//namespace framework
