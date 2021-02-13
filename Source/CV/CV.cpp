#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "Error.h"
#ifdef HAVE_HELMET
#include "CV/Helmet/CVHelmetDetect.h"
#endif//HAVE_HELMET
#ifdef HAVE_SLEEP
#include "CV/Sleep/CVSleepDetect.h"
#endif//HAVE_SLEEP
#ifdef HAVE_PHONE
#include "CV/Phone/CVPhoneDetect.h"
#endif//HAVE_PHONE
#ifdef HAVE_FACE
#include "CV/Face/CVFaceDetect.h"
#endif//HAVE_FACE
#include "CV/CV.h"

namespace framework
{
	namespace ai
	{
		CV::CV() : cv{ nullptr }
		{}
		CV::~CV()
		{
			boost::checked_delete(cv);
		}

		int CV::initCV(void* param /* = nullptr */)
		{
			int e{ param ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
#ifdef HAVE_HELMET
				CVHelmetDetect* hp{ 
					new(std::nothrow) CVHelmetDetect(
						boost::bind(&CV::afterCVResultOutputCallback, this, _1)) };
				if (hp)
				{
					hp->init(param);
					cv = hp;
				}
#endif//HAVE_HELMET
#ifdef HAVE_SLEEP
				CVSleepDetect* sp{
					new(std::nothrow) CVSleepDetect(
						boost::bind(&CV::afterCVResultOutputCallback, this, _1)) };
				if (sp)
				{
					sp->init(param);
					cv = sp;
				}
#endif//HAVE_SLEEP
#ifdef HAVE_PHONE
				CVPhoneDetect* hp{
					new(std::nothrow) CVPhoneDetect(
						boost::bind(&CV::afterCVResultOutputCallback, this, _1)) };
				if (hp)
				{
					hp->init(param);
					cv = hp;
				}
#endif//HAVE_PHONE
#ifdef HAVE_FACE
				CVFaceDetect* fp{
					new(std::nothrow) CVFaceDetect(
						boost::bind(&CV::afterCVResultOutputCallback, this, _1)) };
				if (fp)
				{
					fp->init(param);
					cv = fp;
				}
#endif//HAVE_FACE
			}

			return cv ? eSuccess : eBadNewObject;
		}

		int CV::setParam(void* param /* = nullptr */)
		{
			int e{ param ? eSuccess : eInvalidParameter };

			//仅人脸检测有效
#ifdef HAVE_FACE
			CVFaceDetect* fp{ reinterpret_cast<CVFaceDetect*>(cv) };
			if (fp)
			{
				e = fp->setFaceImage(param);
			}
#endif//HAVE_FACE

			return e;
		}

		int CV::inputData(DataPtr data)
		{
			int e{ 
				data && cv ? eSuccess : eInvalidParameter };

			if (eSuccess == e)
			{
#ifdef HAVE_HELMET
				CVHelmetDetect* cvhd{ reinterpret_cast<CVHelmetDetect*>(cv) };
				if (cvhd)
				{
					e = cvhd->inputData(data);
				}
#endif//HAVE_HELMET
#ifdef HAVE_SLEEP
				CVSleepDetect* cvsd{ reinterpret_cast<CVSleepDetect*>(cv) };
				if (cvsd)
				{
					e = cvsd->inputData(data);
				}
#endif//HAVE_SLEEP
#ifdef HAVE_PHONE
				CVPhoneDetect* cvpd{ reinterpret_cast<CVPhoneDetect*>(cv) };
				if (cvpd)
				{
					e = cvpd->inputData(data);
				}
#endif//HAVE_PHONE
#ifdef HAVE_FACE
				CVFaceDetect* cvfd{ reinterpret_cast<CVFaceDetect*>(cv) };
				if (cvfd)
				{
					e = cvfd->inputData(data);
				}
#endif//HAVE_FACE
			}

			return e;
		}

		void CV::afterCVResultOutputCallback(DataPtr data)
		{
			if (data)
			{
				afterCVResultOutputNotification(data);
			}
		}
	}//namespace ai
}//namespace framework
