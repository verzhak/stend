
#include "stat/stat.hpp"

CMeasure::CMeasure()
{
	CScriptEngine::p_measure = this; // TODO Костыль
}

void CMeasure::init()
{
	__time.clear();
	__memory.clear();
	__modules.clear();
}

void CMeasure::operator()(const double time, const uint64_t memory)
{
	__time.append(time);
	__memory.append(memory);
}

void CMeasure::quality(const unsigned frame_ind, const double quality)
{
	__quality[frame_ind] = quality;
}

void CMeasure::run_module(const QString module_name)
{
	if(! __modules.contains(module_name))
		__modules[module_name] = 0;

	__modules[module_name] ++;
}

// ############################################################################ 

CStat::CStat(CMeasure & measure) :
	__measure(measure)
{
	;
}

void CStat::display()
{
	try
	{
		QProcess process;
		QStringList args;
		QTemporaryFile data_fl;
		QTemporaryFile tmp_script_fl("XXXXXX.r");
		QFile script_fl(":/scripts/" + name_en());

		throw_if(! data_fl.open(), "Не удалось создать временный файл для данных");
		save_data(data_fl);
		throw_if(! data_fl.flush(), "Не удалось сохранить временный файл для данных");

		throw_if(! tmp_script_fl.open(), "Не удалось создать временный файл для модуля");
		throw_if(! script_fl.open(QIODevice::ReadOnly | QIODevice::Text), "Не удалось открыть скрипт");

		const QByteArray data = script_fl.readAll();

		throw_if(data.isEmpty(), "Не удалось прочитать скрипт");
		throw_if(tmp_script_fl.write(data) == -1, "Не удалось записать данные во временный файл для модуля");
		throw_if(! tmp_script_fl.flush(), "Не удалось сохранить данные во временном файле для модуля");

		args << tmp_script_fl.fileName() << data_fl.fileName();

		process.start("Rscript", args);
		process.waitForFinished();
		throw_if(process.exitCode(), "Интерпретатор завершился с ошибкой");
	}
	catch(...)
	{
		;
	}
}

void CStat::save(const QString fname)
{
	try
	{
		QString real_fname = fname;

		if(real_fname == "")
			real_fname = display::get_fname_to_save_stat();

		QFile fl(real_fname);

		throw_if(! fl.open(QIODevice::WriteOnly | QIODevice::Text), "Не удалось открыть результирующий файл");
		save_data(fl);
	}
	catch(...)
	{
		;
	}
}

// ############################################################################ 

void CTimeStat::save_data(QFile & fl)
{
	const unsigned frame_num = __measure.__time.size();
	unsigned v;
	QTextStream stream;

	stream.setDevice(& fl);
	stream.setRealNumberPrecision(11);
	stream << "frame" << "\t" << "time" << endl;

	for(v = 0; v < frame_num; v++)
		stream << v << "\t" << __measure.__time[v] << endl;
}

void CMemoryStat::save_data(QFile & fl)
{
	const unsigned frame_num = __measure.__time.size();
	unsigned v;
	QTextStream stream;

	stream.setDevice(& fl);
	stream << "frame" << "\t" << "memory" << endl;

	for(v = 0; v < frame_num; v++)
		stream << v << "\t" << __measure.__memory[v] << endl;
}

void CTimeMemoryStat::save_data(QFile & fl)
{
	const unsigned frame_num = __measure.__time.size();
	unsigned v;
	QTextStream stream;

	stream.setDevice(& fl);
	stream << "frame" << "\t" << "time" << "\t" << "memory" << endl;

	for(v = 0; v < frame_num; v++)
		stream << v << "\t" << __measure.__time[v] << "\t" << __measure.__memory[v] << endl;
}

void CCallStat::save_data(QFile & fl)
{
	QTextStream stream;

	stream.setDevice(& fl);
	stream << "module" << "\t" << "call" << endl;

	for(auto & module_name : __measure.__modules.keys())
		stream << module_name << "\t" << __measure.__modules[module_name] << endl; // TODO Табуляция в названии модуля будет обработана некорректно
}

void CCallByFirstParameterStat::save_data(QFile & fl)
{
	QTextStream stream;

	stream.setDevice(& fl);
	stream << "TODO_head" << endl;

	qTODO() << "TODO CCallByFirstParameterStat";
}

void CCallBySecondParameterStat::save_data(QFile & fl)
{
	QTextStream stream;

	stream.setDevice(& fl);
	stream << "TODO_head" << endl;

	qTODO() << "TODO CCallBySecondParameterStat";
}

void CCombineQualityStat::save_data(QFile & fl)
{
	const unsigned frame_num = __measure.__time.size();
	unsigned v;
	QTextStream stream;

	stream.setDevice(& fl);
	stream.setRealNumberPrecision(11);
	stream << "frame" << "\t" << "quality" << endl;

	for(auto & frame_ind : __measure.__quality.keys())
		stream << frame_ind << "\t" << __measure.__quality[frame_ind] << endl;
}

