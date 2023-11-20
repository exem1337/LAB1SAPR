#include <uf.h> //файл описаний общих функций
#include <uf_curve.h> //файл описаний функций кривых
#include <uf_csys.h> //файл описаний функций работы с координатами
#include <uf_defs.h>
#include <uf_ui.h>
#include <stdio.h>
#include <uf_part.h>
#include <uf_modl.h>
#include <uf_object_types.h>
#include <uf_obj.h>
#include <uf_mtx.h>

void circle(float x, float y, float radius) {
	tag_t arc_id, wcs_tag; 
	UF_CURVE_arc_t arc_coords; 

	arc_coords.start_angle = 0.0; 
	arc_coords.end_angle = 360.0 * DEGRA; 
	arc_coords.arc_center[0] = x; //координата центра Х
	arc_coords.arc_center[1] = y; //координата центра Y
	arc_coords.arc_center[2] = 0.; //координата центра Z
	arc_coords.radius = radius; //радиус окружности

	UF_CSYS_ask_wcs(&wcs_tag); //получение абсолютных координат
	//"перенос" абсолютных координат на создаваемую окружность
	UF_CSYS_ask_matrix_of_object(wcs_tag, &arc_coords.matrix_tag);
	UF_CURVE_create_arc(&arc_coords, &arc_id);
}

void line(float startX, float startY, float endX, float endY) {
	tag_t entid = 0; 
	UF_CURVE_line_t line_coords; 

	line_coords.start_point[0] = startX; // X1
	line_coords.start_point[1] = startY; // Y1
	line_coords.start_point[2] = 0.; // Z1
	line_coords.end_point[0] = endX; // X2
	line_coords.end_point[1] = endY; // Y2
	line_coords.end_point[2] = 0.; // Z2

	UF_CURVE_create_line(&line_coords, &entid);
}

void rectangle(float startX, float startY, float sideWitdh) {
	// функция строит квадрат от верхей левой его точки
		
	tag_t entid = 0;
	UF_CURVE_line_t line1_coords;
	UF_CURVE_line_t line2_coords;
	UF_CURVE_line_t line3_coords;
	UF_CURVE_line_t line4_coords;

	//верхняя линия
	line1_coords.start_point[0] = startX;// X1
	line1_coords.start_point[1] = startY;// Y1
	line1_coords.start_point[2] = 0.;// Z1
	line1_coords.end_point[0] = startX + sideWitdh;// X2
	line1_coords.end_point[1] = startY;// Y2
	line1_coords.end_point[2] = 0.;// Z2
	UF_CURVE_create_line(&line1_coords, &entid);

	//правая линия
	line2_coords.start_point[0] = startX + sideWitdh;// X1
	line2_coords.start_point[1] = startY;// Y1
	line2_coords.start_point[2] = 0.;// Z1
	line2_coords.end_point[0] = startX + sideWitdh;// X2
	line2_coords.end_point[1] = startY - sideWitdh;// Y2
	line2_coords.end_point[2] = 0.;// Z2
	UF_CURVE_create_line(&line2_coords, &entid);

	//нижняя линия
	line3_coords.start_point[0] = startX;// X1
	line3_coords.start_point[1] = startY - sideWitdh;// Y1
	line3_coords.start_point[2] = 0.;// Z1
	line3_coords.end_point[0] = startX + sideWitdh;// X2
	line3_coords.end_point[1] = startY - sideWitdh;// Y2
	line3_coords.end_point[2] = 0.;// Z2
	UF_CURVE_create_line(&line3_coords, &entid);

	//левая линия
	line4_coords.start_point[0] = startX;// X1
	line4_coords.start_point[1] = startY;// Y1
	line4_coords.start_point[2] = 0.;// Z1
	line4_coords.end_point[0] = startX;// X2
	line4_coords.end_point[1] = startY - sideWitdh;// Y2
	line4_coords.end_point[2] = 0.;// Z2
	UF_CURVE_create_line(&line4_coords, &entid);
}

void createArc(
	double center[3], // точка центра { x, y, z }
	float radius, // радиус
	int startAngle, // стартовый угол
	int endAngle // конечный угол
)
{
	tag_t arc_id, wcs_tag;
	UF_CURVE_arc_t arc; 

	arc.start_angle = startAngle * DEGRA;
	arc.end_angle = endAngle * DEGRA;
	arc.arc_center[0] = center[0];
	arc.arc_center[1] = center[1];
	arc.arc_center[2] = center[2];
	arc.radius = radius;

	UF_CSYS_ask_wcs(&wcs_tag); 
	UF_CSYS_ask_matrix_of_object(wcs_tag, &arc.matrix_tag);
	UF_CURVE_create_arc(&arc, &arc_id);
}

void lab2() {
	char msg[128] = "Задайте плоскость для создания окружности";
	
	// запуск диалога создания плоскости

	for (int i = 0; i < 5; i++) {
		double plane_matrix[9] = {
			0.866025, 0.5, 0.0 + i * 50,
			-0.5, 0.866025, 0.0,
			0.0, 0.0, 1.0 
		};

		double plane_origin[3] = { 0, 10, 20 };
		tag_t mtx_id = i + 1;
		tag_t csys_id = i + .5;
		tag_t arc_id = i + .75;

		UF_CURVE_arc_t arc;
		arc.start_angle = 0.0;
		arc.end_angle = TWOPI;
		arc.radius = 50;

		UF_CSYS_create_matrix(plane_matrix, &mtx_id);
		UF_CSYS_create_csys(plane_origin, mtx_id, &csys_id);
		UF_CSYS_set_wcs(csys_id);

		arc.matrix_tag = mtx_id;
		UF_MTX3_vec_multiply(plane_origin, plane_matrix, arc.arc_center);
		arc.arc_center[0] += 30.0;
		arc.arc_center[1] += 25.0;
		UF_CURVE_create_arc(&arc, &arc_id);
	}
}

void doSomeShit() {
	char a[] = "30.0";
	char b[] = "80.0";
	char t_a[] = "0.0";
	/* Определяется массив линий с координатами их
концов, используемых в эскизе выдавливаемого сечения */
	UF_CURVE_line_t line[3] = {
	{{10.0,0.0,0.0}, {10.0,40.0,0.0}},
	{{10.0,0.0,0.0}, {40.0,0.0,0.0}},
	{{10.0,40.0,0.0}, {40.0,40.0,0.0}} };
	UF_CURVE_arc_t arc; //структура данных дуги
	tag_t objarray[4], //теги для трех линий и дуги
		wcs_tag; //тег для системы координат
	int i;
	//параметры операции выдавливания
	//расстояние начала и расстояние окончания выдавливания

		char* limit[2] = { a, b };
	//орты вектора направления выдавливания
	double direction[3] = { 0.0, 0.0, 1.0 };
	//признак создания самостоятельного тела
	UF_FEATURE_SIGN create = UF_NULLSIGN;
	//заготовки указателей на перечии объектов
	uf_list_p_t loop_list, //Список объектов, подлежащих выдавливанию (элементы сечения)
		features; //Список созданных идентификаторов объектов
	//угол конусности в градусах
	char* taper_angle = t_a;
	//параметр не используется
	double ref_pt[3];

		// задание пераметров дуги в сечении
		arc.start_angle = -90.0 * DEGRA;
		arc.end_angle = 90.0 * DEGRA;
		arc.arc_center[0] = 40.0;
		arc.arc_center[1] = 20.0;
		arc.arc_center[2] = 0.0;
		arc.radius = 20.0;
		for (i = 0; i < 3; i++) //построение трех линий сечения
			UF_CURVE_create_line(&line[i], &objarray[i]);
		//получение матрицы абсолютных координат
		UF_CSYS_ask_wcs(&wcs_tag);
		//связывание матрицы поворота дуги с абсолютными координатами
		UF_CSYS_ask_matrix_of_object(wcs_tag, &arc.matrix_tag);
		//построение дуги сечения
		UF_CURVE_create_arc(&arc, &objarray[i]);
		//создание пустого перечия объектов
		UF_MODL_create_list(&loop_list);
		//заполнение перечия тремя линиями и одной дугой
		for (i = 0; i < 4; i++)
			UF_MODL_put_list_item(loop_list, objarray[i]);
	
			//создание операции выдавливания
			UF_MODL_create_extruded(loop_list, taper_angle, limit,
				ref_pt, direction, create, &features);
}

void labserega() {
	line(0.0, 0.0, 100, 0);
	line(0.0, 10, 100, 10);

	line(100, 0, 100, 10);
	line(100, 10, 92.5, 10);
	line(92.5, 10, 75, 80);

	line(75, 80, 67.5, 80);
	line(25, 80, 32.5, 80);

	line(25, 80, 7.5, 10);
	line(7.5, 10, 0, 10);
	line(0, 10, 0, 0);

	createArc(
		new double[3] { 50, 80, 0 },
		25,
		180,
		360
	);

	createArc(
		new double[3] { 50, 80, 0 },
		17.5,
		180,
		360
	);

	circle(50, 40, 7.5);
}

void lab2Serega() {
	line(0, 0, 1120, 0);
	line(1120, 0, 1120, 73);
	line(0, 73, 1120, 73);
	line(0, 0, 0, 73);


	#define NUMBER_POINTS 5 //количество точек в сплайне зададим пять
	int degree = 3; // строить будем сплайн третьей степени
	int periodicity = 0; // не периодичный сплайн
	int num_points = NUMBER_POINTS;
	// объявление массива структур с данными по точкам
	UF_CURVE_pt_slope_crvatr_t point_data[NUMBER_POINTS];
	// зададим массив параметров точек
	double parameters[NUMBER_POINTS] = { 0.00, 0.17, 0.32, 0.45, 1.29 };
	// зададим массив координат точек
	double points[3 * NUMBER_POINTS] = {
	0, 73, 0,
	112, 87, 0,
	224, 101, 0,
	346, 115, 0,
	468, 129, 0, };
	// зададим массив методов задания касательных
	int slopeTypes[NUMBER_POINTS] = {
	UF_CURVE_SLOPE_DIR,
	UF_CURVE_SLOPE_AUTO,
	UF_CURVE_SLOPE_NONE,
	UF_CURVE_SLOPE_DIR,
	UF_CURVE_SLOPE_VEC };
	// зададим массив векторов касательных в точках
	double slopeVecs[3 * NUMBER_POINTS] = {
	1.2300, 55.0506, 0,
	0.0000, 660.0000, 0,
	0.0000, 70.0000, 0,
	0.5000, 1.0000, 0,
	1.0000,-2.0000, 0 };
	// зададим массив методов задания кривизн в точках
	int crvatrTypes[NUMBER_POINTS] = {
	UF_CURVE_CRVATR_NONE,
	UF_CURVE_CRVATR_AUTO_DIR,
	UF_CURVE_CRVATR_NONE,
	UF_CURVE_CRVATR_VEC,
	UF_CURVE_CRVATR_VEC };
	// зададим массив векторов кривизн в точках
	double crvatrVecs[3 * NUMBER_POINTS] = {
	0.0000, 67.0000, 0.0000,
	1.0000, 42.5780, 0,
	0.0000, 30.0000, 0,
	1.0000, 71.0000, 0,
	-1.0000, 91.0000, 0 };
	int i, save_def_data = 1;
	tag_t spline_tag;
	if (!UF_initialize())
	{ /* выполним цикл переноса данных из заданных массивов в
	соответствующие поля структуры данных сплайна */
		for (i = 0; i < NUMBER_POINTS; i++)
		{
			point_data[i].point[0] = points[3 * i];
			point_data[i].point[1] = points[3 * i + 1];
			point_data[i].point[2] = points[3 * i + 2];
			point_data[i].slope_type = slopeTypes[i];
			point_data[i].slope[0] = slopeVecs[3 * i];
			point_data[i].slope[1] = slopeVecs[3 * i + 1];
			point_data[i].slope[2] = slopeVecs[3 * i + 2];
			point_data[i].crvatr_type = crvatrTypes[i];
			point_data[i].crvatr[0] = crvatrVecs[3 * i];
			point_data[i].crvatr[1] = crvatrVecs[3 * i + 1];
			point_data[i].crvatr[2] = crvatrVecs[3 * i + 2];
		}
		// создание сплайна
		UF_CURVE_create_spline_thru_pts(degree,
			periodicity,
			num_points,
			point_data,
			parameters,
			save_def_data,
			&spline_tag);
	}
}

void ufusr(char* param, int* retcode, int paramLen)
{
	if (UF_initialize()) return;

	//circle(54., 25., 7.5f);

	//line(0., 0., 115., 0.);
	//line(115., 0., 115., 10.);
	//line(115., 10., 107., 10.);
	//line(107., 10., 107., 40.);
	//line(107., 40., 115., 40.);
	//line(115., 40., 115., 50.);
	//line(115., 50., 0., 50.);
	//line(0., 50., 0., 0.);

	//line(27., 0., 27., 10.);
	//line(27., 50., 27., 40.);
	//line(81., 0., 81., 50.);

	//// левый квадрат
	//line(10., 10., 10., 40.);
	//line(10., 10., 34., 10.);
	//line(10., 40., 34., 40.);
	//line(34., 40., 34., 10.);

	////центральный большой квадрат
	//rectangle(34., 45., 40.);

	////центральный малый квадрат
	//rectangle(39., 40., 30.);

	//rectangle(51.5, 6.25 + 28.0, 28.);
	//circle(51.5 + (28. / 2), 6.25 + (28. / 2), 9.5f);

	////Container
	//line(0., 0., 115., 0.);
	//line(0., 0., 0., 40.);
	//line(22., 0., 22., 40.);
	//line(0., 40., 115., 40.);

	////left edge container
	//line(115., 0., 115., 12.5);
	//line(115., 12.5, 115. - 15., 12.5);
	//line(115. - 15., 12.5, 115. - 15., 12.5 + 15.0);
	//line(115. - 15., 12.5 + 15.0, 115., 12.5 + 15.0);
	//line(115., 15. + 12.5, 115., 15. + 12.5 + 12.5);

	//rectangle(5., 14. + 12., 12.);

	// doSomeShit();

	lab2Serega();

	UF_terminate();
}

int ufusr_ask_unload(void) //функция выхода из приложения
{
	return (UF_UNLOAD_IMMEDIATELY);
}

