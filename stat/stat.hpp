
#ifndef STAT_HPP
#define STAT_HPP

#include "all.hpp"
#include "script_engine/script_engine.hpp"
#include "main_loop/display.hpp"

class CTimeStat;
class CMemoryStat;
class CTimeMemoryStat;
class CCallStat;
class CCallByFirstParameterStat;
class CCallBySecondParameterStat;
class CCombineQualityStat;

class CMeasure
{
	protected:

		QList<double> __time;
		QList<uint64_t> __memory;
		QMap<unsigned, double> __quality;
		QMap<QString, unsigned> __modules;

		friend class CTimeStat;
		friend class CMemoryStat;
		friend class CTimeMemoryStat;
		friend class CCallStat;
		friend class CCallByFirstParameterStat;
		friend class CCallBySecondParameterStat;
		friend class CCombineQualityStat;

	public:

		CMeasure();

		void init();
		void operator()(const double time, const uint64_t memory);
		void quality(const unsigned frame_ind, const double quality);
		void run_module(const QString module_name);
};

class CStat : public QObject
{
	Q_OBJECT

	protected:

		const CMeasure & __measure;

		virtual void save_data(QFile & fl) = 0;

	public:

		CStat(CMeasure & measure);

		virtual QString name_en() const = 0;
		virtual QString name_ru() const = 0;

	public slots:

		virtual void display();
		void save(const QString fname = "");
};

// ############################################################################ 

class CTimeStat : public CStat
{
	Q_OBJECT

	protected:

		void save_data(QFile & fl);

	public:

		CTimeStat(CMeasure & measure) : CStat(measure) { ; };

		QString name_en() const { return "time"; };
		QString name_ru() const { return trUtf8("Время выполнения алгоритма на кадре"); };
};

class CMemoryStat : public CStat
{
	Q_OBJECT

	protected:

		void save_data(QFile & fl);

	public:

		CMemoryStat(CMeasure & measure) : CStat(measure) { ; };

		QString name_en() const { return "memory"; };
		QString name_ru() const { return trUtf8("Объем памяти, использованной для выполнения алгоритма на кадре"); };
};

class CTimeMemoryStat : public CStat
{
	Q_OBJECT

	protected:

		void save_data(QFile & fl);

	public:

		CTimeMemoryStat(CMeasure & measure) : CStat(measure) { ; };
};

class CTimeMemory2DStat : public CTimeMemoryStat
{
	Q_OBJECT

	public:

		CTimeMemory2DStat(CMeasure & measure) : CTimeMemoryStat(measure) { ; };

		virtual QString name_en() const { return "time_memory_2D"; };
		virtual QString name_ru() const { return trUtf8("Отношение объема использованной памяти к времени выполнения алгоритма на кадре"); };
};

class CTimeMemory3DStat : public CTimeMemoryStat
{
	Q_OBJECT

	public:

		CTimeMemory3DStat(CMeasure & measure) : CTimeMemoryStat(measure) { ; };

		QString name_en() const { return "time_memory_3D"; };
		QString name_ru() const { return trUtf8("Время выполнения и объем использованной памяти для выполнения алгоритма на кадре"); };
};

class CCallStat : public CStat
{
	Q_OBJECT

	protected:

		void save_data(QFile & fl);

	public:

		CCallStat(CMeasure & measure) : CStat(measure) { ; };

		QString name_en() const { return "call"; };
		QString name_ru() const { return trUtf8("Количество вызовов пользовательских алгоритмов покадрово"); };
};

class CCallByFirstParameterStat : public CStat
{
	Q_OBJECT

	protected:

		void save_data(QFile & fl);

	public:

		CCallByFirstParameterStat(CMeasure & measure) : CStat(measure) { ; };

		QString name_en() const { return "call_first"; };
		QString name_ru() const { return trUtf8("Количество вызовов пользовательских алгоритмов покадровой по значению первого параметра"); };
};

class CCallBySecondParameterStat : public CStat
{
	Q_OBJECT

	protected:

		void save_data(QFile & fl);

	public:

		CCallBySecondParameterStat(CMeasure & measure) : CStat(measure) { ; };

		QString name_en() const { return "call_second"; };
		QString name_ru() const { return trUtf8("Количество вызовов пользовательских алгоритмов покадровой по значению второго параметра"); };
};

class CCombineQualityStat : public CStat
{
	Q_OBJECT

	protected:

		void save_data(QFile & fl);

	public:

		CCombineQualityStat(CMeasure & measure) : CStat(measure) { ; };

		QString name_en() const { return "combine_quality"; };
		QString name_ru() const { return trUtf8("Качество совмещения изображений"); };
};

#endif

