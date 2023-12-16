﻿#include <uf.h> //файл описаний общих функций
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
#include <uf_modl_types.h>
#include <uf_sket_types.h>
#include <uf_sket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define NUMBER_POINTS 10 // количество точек в сплайне

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

void line(float startX, float startY, float endX, float endY, float startZ = 0, float endZ = 0) {
	tag_t entid = 0; 
	UF_CURVE_line_t line_coords; 

	line_coords.start_point[0] = startX; // X1
	line_coords.start_point[1] = startY; // Y1
	line_coords.start_point[2] = startZ; // Z1
	line_coords.end_point[0] = endX; // X2
	line_coords.end_point[1] = endY; // Y2
	line_coords.end_point[2] = endZ; // Z2

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
	// данные для окружности
	tag_t arc_id, wcs_tag; //тэги окружности
	//и мировой системы координат
	UF_CURVE_arc_t arc_coords; //структура свойств дуги
	// объявление данных для матрицы поворота
	//зададим угол 60° и пересчитаем его в радианы
	double ugol_Y = 0;
	// объявим вектора осей Х и У
	double vec_X[3] = { 1, 0, 0 }, vec_Y[3] = { 0, 1, 0 };
	//объявим матрицы поворотов:
	double mtxP[9], //вокруг оси Х
		mtyP[9], //вокруг оси У
		mt[9]; //суммарную матрицу поворота
	//объявим теги систем координат
	tag_t teg_wcs, csys_id;
	// объявим точку начала координат
	double center[3] = { 0,0,0 };

	// начало кода построения окружности
	arc_coords.start_angle = 0.0; //начальный угол окружности
	arc_coords.end_angle = 360.0 * DEGRA; //конечный угол
	arc_coords.arc_center[0] = 0.0; //координата центра Х
	arc_coords.arc_center[1] = 0.0; //координата центра Y
	arc_coords.arc_center[2] = 0.0; //координата центра Z
	arc_coords.radius = 30.0; //радиус окружности

	for (int i = 0; i <= 4; i++) {
		// начало кода работы с матрицами
		// получим матрицу поворота на 60 вокруг оси Х
		UF_MTX3_rotate_about_axis(vec_X, ugol_Y, mtxP);
		// создадим тег суммарной матрицы поворота
		UF_CSYS_create_matrix(mtxP, &teg_wcs);
		// создадим тег системы координат на базе полученной матрицы
		UF_CSYS_create_csys(center, teg_wcs, &csys_id);
		// установим на экране созданную рабочую систему координат
		UF_CSYS_set_wcs(csys_id);
		UF_CSYS_ask_wcs(&wcs_tag); //получение абсолютных координат
		//"перенос" абсолютных координат на создаваемую окружность
		UF_CSYS_ask_matrix_of_object(wcs_tag, &arc_coords.matrix_tag);
		//построение окружности
		UF_CURVE_create_arc(&arc_coords, &arc_id);

	//	int degree = 3; // степень сплайна
	//	int periodicity = 0; // сплайн не периодичный
	//	int num_points = 3;
	//	int save_def_data = 1;
	//	tag_t spline_tag = i;
	//	// заполнение структуры параметров сплайна
	//	UF_CURVE_pt_slope_crvatr_t point_data[3] {
	//	{ 
	//		{0, 0, 0}, // координаты первой точки
	//		UF_CURVE_SLOPE_DIR, 
	//		{ 0.0, 1.0, 0.0000 }, 
	//		UF_CURVE_CRVATR_NONE, 
	//		{ 0.0000, 0.0000, 0.0000 }
	//	},
	//	{ 
	//		{0, 0, 0},
	//		UF_CURVE_SLOPE_NONE, { 0, 1, 0 },
	//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }
	//	},
	//	{ 
	//		{0, 0, 0},
	//		UF_CURVE_SLOPE_DIR,{0.0, 1, 0.0},
	//		UF_CURVE_CRVATR_NONE,{0.0, 0.0, 0.0}
	//	}};

	//	UF_CURVE_create_spline_thru_pts(
	//		degree,
	//		periodicity,
	//		num_points,
	//		point_data,
	//		NULL, //с заданием монотонности по умолчанию
	//		save_def_data,
	//		&spline_tag
	//	);

	//line(0, 0, 0, 100, 0, ugol_Y * DEGRA);


	ugol_Y += 45 * DEGRA;
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

void lab3() {
	UF_FEATURE_SIGN sign = UF_NULLSIGN;
	//это тело должно начинаться в заданных координатах Х, У, Z 
	double cyl_orig[3] = { 0, 0, 0 };
	char height[125] = "70"; //высоту цилиндра зададим в 70 мм 
	char diam[125] = "50"; //диаметр-в 50 мм 
	//и орты направления главной диагонали координат будут по 1 
	double direction[3] = { 0, 0, 1 };
	tag_t cyl_obj; //определим переменную для будущего тега цилиндра
	if (!UF_initialize()) {
		//выполним построение цилиндра 
		UF_MODL_create_cyl1(sign, cyl_orig, height, diam, direction, &cyl_obj);
	}

	UF_FEATURE_SIGN sign1 = UF_NULLSIGN;
	//это тело должно начинаться в заданных координатах Х, У, Z 
	double cyl_orig1[3] = { 0, 0, 0 };
	char height1[125] = "70"; //высоту цилиндра зададим в 70 мм 
	char diam1[125] = "50"; //диаметр-в 50 мм 
	//и орты направления главной диагонали координат будут по 1 
	double direction1[3] = { 0, 1, 0 };
	tag_t cyl_obj1; //определим переменную для будущего тега цилиндра
	if (!UF_initialize()) {
		//выполним построение цилиндра 
		UF_MODL_create_cyl1(sign1, cyl_orig1, height1, diam1, direction1, &cyl_obj1);
	}

	UF_FEATURE_SIGN sign2 = UF_NULLSIGN;
	//это тело должно начинаться в заданных координатах Х, У, Z 
	double cyl_orig2[3] = { 0, 0, 0 };
	char height2[125] = "70"; //высоту цилиндра зададим в 70 мм 
	char diam2[125] = "50"; //диаметр-в 50 мм 
	//и орты направления главной диагонали координат будут по 1 
	double direction2[3] = { 1, 0, 0 };
	tag_t cyl_obj2; //определим переменную для будущего тега цилиндра
	if (!UF_initialize()) {
		//выполним построение цилиндра 
		UF_MODL_create_cyl1(sign2, cyl_orig2, height2, diam2, direction2, &cyl_obj2);
	}
}
char mes[131];//переменная для текстовых сообщений

void PatternAndChildrenSub(tag_t ftag)
{
	int j, k, num_parents, num_children;
	tag_t* parent_array, * children_array;
	char* feature_type;
	int num_cons;
	tag_t* con_tags;
	uf_list_p_t object_list;
	UF_SKET_con_type_t con_type;
	char name[UF_OBJ_NAME_LEN + 1];
	//определение родителей и детей 
	UF_MODL_ask_feat_relatives(ftag, &num_parents,
		&parent_array, &num_children, &children_array);
	//вывод в информационное окно переченя родителей
	sprintf_s(mes, " Родителей=%d шт.\n", num_parents);
	UF_UI_write_listing_window(mes);
	for (j = 0; j < num_parents; j++) //цикл по количеству родителей
	{//определение типа родителя
		UF_MODL_ask_feat_type(parent_array[j], &feature_type);
		//вывод типа родителя в информационное окно
		sprintf_s(mes, "%s\n", feature_type);
		UF_UI_write_listing_window(mes);
		sprintf_s(mes, "%s\n", "тип родителя");
		UF_UI_write_listing_window(mes);
		//если родитель ЭСКИЗ
		if (strcmp(feature_type, "SKETCH") == 0) {
			//то дополнительно запросим состав элементов эскиза
			UF_SKET_ask_feature_sketches(parent_array[j], &object_list);
			//на первом элементе эскиза проверим наличие размеров
			UF_SKET_ask_constraints_of_sketch(object_list->eid, (UF_SKET_con_class_t)UF_SKET_dim_cons,
				&num_cons, &con_tags);
			//выводим информацию о размерах эскиза на экран
			sprintf_s(mes, " Размерных элементов скетча = %d шт.\n", num_cons);
			UF_UI_write_listing_window(mes);
			//очищаем все полученные динамические переменные
			UF_free(con_tags);
			UF_MODL_delete_list(&object_list);
		}
		//очищаем полученную в начале процедуры переменную типа родителя 
		UF_free(feature_type);
	}
	//аналогично составляем перечень «детей»
	//выводим на экран число «детей» 
	sprintf_s(mes, " Количество детей=%d шт.\n", num_children);
	UF_UI_write_listing_window(mes);
	//и для каждого «ребёнка»
	for (j = 0; j < num_children; j++) {//определяем его тип
		UF_MODL_ask_feat_type(children_array[j], &feature_type);
		//и выводим этот тип на экран
		sprintf_s(mes, "%s\n", feature_type);
		UF_UI_write_listing_window(mes);
		//освобождаем переменную с полученным типом
		UF_free(feature_type);
	}
	//выводим декоративную черту для отделения одного элемента от другого 
	UF_UI_write_listing_window("---------------------- - \n");
	//освобождаем массивы «родителей» и «детей»
	UF_free(parent_array);
	UF_free(children_array);
}

void CylinderSub(tag_p_t ftags, int nt) {
	int i;
	int edit = 1;
	char* diameter;
	char* height;
	//цикл по количеству цилиндров
	for (i = 0; i < nt; i++)
	{ //функция получения параметров цилиндра
		UF_MODL_ask_cylinder_parms(ftags[i], edit, &diameter, &height);
		//вывод на экран текста полученного параметра
		sprintf_s(mes, " Диаметр : %s мм Высота : %s мм\n", diameter, height);
		//освобождение полученных динамических переменных
		UF_UI_write_listing_window(mes);
		UF_free(diameter); UF_free(height);
		//вызов процедуры определения родителей и детей
		PatternAndChildrenSub(ftags[i]);
	} return;
}


void ShpereDelete(tag_t* next_tag)
{
	int body_type, cnt;
	tag_t body, view;
	int type, subtype;
	int feat_count;
	UF_MODL_features_p_t* features_node;
	int count = 0;
	tag_t tags[100];
	tag_t objt;
	char* tipN;
	int Type, SubType, Body_Type;
	char mes[131];//переменная для текстовых сообщений

	do
	{
		*next_tag = UF_OBJ_cycle_all(UF_PART_ask_display_part(), *next_tag);
		if (*next_tag != NULL_TAG)
		{
			UF_OBJ_ask_type_and_subtype(*next_tag, &Type, &SubType);
			if (SubType == UF_solid_body_subtype)
			{
				UF_MODL_ask_body_type(*next_tag, &Body_Type);
				if (Body_Type == UF_MODL_SOLID_BODY)
				{
					tags[count] = *next_tag;
					count++;
				}
			}



		}

	} while (*next_tag != NULL_TAG);

	for (int i = 0; i < count; i++)
	{
		UF_MODL_ask_body_features(tags[i], &feat_count, &features_node);
		for (int j = 0; j < feat_count; j++)
		{//запомнить текущее количество и тип элемента
			cnt = features_node[j]->feat_count;
			tipN = features_node[j]->feat_type;


			//выбрать по типу элемента процедуру для его обработки
			//if (strcmp(tip, "SPHERE") == 0)
			if (strcmp(tipN, "SPHERE") == 0)
				UF_OBJ_delete_object(tags[i]);

			/*if (strcmp(tip, "BLEND") == 0)
				BlendSub(features_node[i]->feat_tags, cnt);
			if (strcmp(tip, "CYLINDER") == 0)
				CylinderSub(features_node[i]->feat_tags, cnt);*/

		} //освободить память от динамической переменной
		//UF_free(features_node);

	}
	UF_free(features_node);
	/*sprintf_s(mes, "%d", count);
	UF_UI_write_listing_window(mes);*/

}

void lab3Serega() {
	char mes[131];//переменная для текстовых сообщений
	char msg[131] = "Выберите твердое тело";
	int cnt, i; char* tip;
	int feat_count;
	UF_MODL_features_p_t* features_node;
	//переменные для функции выбора тела
	double cursor[3];
	int err = 0, response = 4;
	UF_UI_selection_options_t opts;
	UF_UI_mask_t mask = { UF_solid_type,0, UF_UI_SEL_FEATURE_BODY };
	int body_type;
	tag_t body, view;
	tag_t next_tag = NULL_TAG;
	//tag_t WorkPartTag = UF_PART_ask_display_part();
	/*Главная точка входа*/
	if (!UF_initialize()) {//заполнение структуры для функции выбора тела
		opts.num_mask_triples = 1;
		opts.mask_triples = &mask;
		opts.scope = UF_UI_SEL_SCOPE_WORK_PART;
		//активизация информационного окна системы для вывода текста
		UF_UI_open_listing_window();
		//начать цикл
		//do {//открыть диалог выбора тела
		err = UF_UI_select_single(msg, &opts, &response, &body, cursor, &view);
		//если на диалоге нажата кнопка ОК или ПРИНЯТЬ,
		if ((response == 4) || (response == 5))
		{


			ShpereDelete(&next_tag);

			/*if (tip == "SPHERE")
				SphereSub(features_node[i]->feat_tags, cnt);*/
				/*if (strcmp(tip, "BLEND") == 0)
					BlendSub(features_node[i]->feat_tags, cnt);
				if (strcmp(tip, "CYLINDER") == 0)
					CylinderSub(features_node[i]->feat_tags, cnt);*/

					//убрать цвет выделения с выбранного тела сцены
					//UF_DISP_set_highlight(body, 0);

				//продолжать цикл, пока пользователь нажимает кнопки ОК или ПРИНЯТЬ
		}
		/*} while ((response == 4) || (response == 5));*/
		UF_terminate();
	}
}

//void lab2Serega() {
//	int degree = 3; // степень сплайна
//	int periodicity = 0; // сплайн не периодичный
//	int num_points = NUMBER_POINTS;
//	int save_def_data = 1;
//	tag_t spline_tag;
//	
//	UF_CURVE_pt_slope_crvatr_t point_data[NUMBER_POINTS]
//	{
//		{ {0.0, 0.0, 45.0},
//		UF_CURVE_SLOPE_DIR, { 0.0000, 0.0000, 0.0000 }, 
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }
//		},
//		{ {-112, 14, 45.0000},
//		UF_CURVE_SLOPE_NONE, {0.0000, 0.0000, 0.0000},
//		UF_CURVE_CRVATR_NONE, {0.0000, 0.0000, 0.0000}
//		},
//		{ {-224, 51, 45.0000},
//		UF_CURVE_SLOPE_NONE, { 0.0000, 0.0000, 0.0000 },
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }
//		},
//		{ {-336, 97, 45.0},
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 },
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }
//		},
//		{ {-448, 135, 45.0},
//		UF_CURVE_SLOPE_DIR,{0.0000, 0.0000, 0.0000},
//		UF_CURVE_CRVATR_NONE,{0.0000, 0.0000, 0.0000}
//		},
//		{ {-560, 149, 45.0},
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 },
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }
//		},
//		{ {-672, 135, 45.0},
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 },
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }
//		},
//		{ {-784, 97, 45.0},
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 },
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }
//		},
//		{ {-896, 51, 45.0},
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 },
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }
//		},
//		{ {-1008, 14, 45.0},
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 },
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }
//		},
//		{ {-1120, 0.0, 45.0},
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 },
//		UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }
//		}
//	};
//
//	for (int i = 0; i < NUMBER_POINTS; ++i) {
//		double current_slope = point_data[i].slope;
//		double new_slope = current_slope + angle; // Новый угол наклона
//
//		// Предположим, что у вас есть функция для установки угла наклона
//		set_slope(&point_data[i], new_slope); // Применяем новый угол наклона
//	}
//
//	if (!UF_initialize())
//	{
//		// создание сплайна
//		UF_CURVE_create_spline_thru_pts(degree,
//			periodicity,
//			num_points,
//			point_data,
//			NULL, //с заданием монотонности по умолчанию
//			save_def_data,
//			&spline_tag);
//		line(0., -73., 0, 0, 0, 45);
//		line(0., -73., -1120., -73., 0, 0);
//		line(-1120., -73., -1120., 0., 0, 45);
//		UF_terminate();
//	}
//}

void lab2oleg() {
	int degree = 5; // степень сплайна
	int periodicity = 0; // сплайн не периодичный
	int num_points = NUMBER_POINTS;
	int save_def_data = 1;
	tag_t spline_tag;

	// заполнение структуры параметров сплайна
	UF_CURVE_pt_slope_crvatr_t point_data[NUMBER_POINTS]{
	  { {0.0, 0.0, 0.0}, // координаты первой точки
		UF_CURVE_SLOPE_DIR, /*в ней будет задано направление касательной */
		{ 0.0, 1.0, 0.0000 }, // вектор касательной
		UF_CURVE_CRVATR_NONE, // кривизна задаваться не будет
		{ 0.0000, 0.0000, 0.0000 } },
		// в следующих точках задаются только координаты
		{ {100.0, 0.0, 0.0}, UF_CURVE_SLOPE_NONE, {0.0000, 0.0000, 0.0000}, UF_CURVE_CRVATR_NONE, {0.0000, 0.0000, 0.0000} },
		{ {200.0, 100.0, 0.0000}, UF_CURVE_SLOPE_NONE, { 0.0000, 0.0000, 0.0000 }, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 } },
		{ {300.0, 200.0, 0.0}, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 } },
		{ {400.0, 300.0, 0.0}, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 } },
		{ {500.0, 400.0, 0.0}, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 } },
		{ {600.0, 500.0, 0.0}, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 } },
		{ {700.0, 600.0, 0.0}, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 }, UF_CURVE_CRVATR_NONE, { 0.0000, 0.0000, 0.0000 } },
	};
	// создание сплайна
	UF_CURVE_create_spline_thru_pts(degree, periodicity, num_points, point_data, NULL, //с заданием монотонности по умолчанию
		save_def_data, &spline_tag);
}

void lab1Oleg() {
	line(0, 0, 120, 0);
	line(120, 0, 120, 10);
	line(120, 10, 100, 45);
	line(100, 45, 95, 45);
	line(0, 0, 0, 10);
	line(0, 10, 20, 45);
	line(20, 45, 25, 45);
	
	line(25, 0, 25, 45);
	line(95, 0, 95, 45);

	circle(60, 45, 20);

	double center[3] = { 60, 45, 0.0 }; // Центр дуги
	float radius = 35; // Радиус дуги
	int startAngle = 0; // Стартовый угол в градусах
	int endAngle = 180; // Конечный угол в градусах

	createArc(center, radius, startAngle, endAngle);
}

void ufusr(char* param, int* retcode, int paramLen)
{
	if (UF_initialize()) return;

	lab1Oleg();
	//lab2oleg();
	//lab3();
;
	UF_terminate();
}

int ufusr_ask_unload(void) //функция выхода из приложения
{
	return (UF_UNLOAD_IMMEDIATELY);
}
