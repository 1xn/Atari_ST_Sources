#include    "portab.h"

extern    WORD    contrl[];
extern    WORD    intin[];
extern    WORD    ptsin[];
extern    WORD    intout[];
extern    WORD    ptsout[];

extern    WORD	*pioff, *iioff, *pooff, *iooff;

extern    vdi();

#define i_ptsin(ptr) pioff = ptr
#define i_intin(ptr) iioff = ptr
#define i_intout(ptr) iooff = ptr
#define i_ptsout(ptr) pooff = ptr

extern    i_ptr();
extern    i_ptr2();
extern    m_lptr2();


    WORD
v_arc( handle, xc, yc, rad, sang, eang )
WORD handle, xc, yc, rad, sang, eang;
{
    ptsin[0] = xc;
    ptsin[1] = yc;
    ptsin[2] = 0;
    ptsin[3] = 0;
    ptsin[4] = 0;
    ptsin[5] = 0;
    ptsin[6] = rad;
    ptsin[7] = 0;
    intin[0] = sang;
    intin[1] = eang;

    contrl[0] = 11;
    contrl[1] = 4;
    contrl[3] = 2;
    contrl[5] = 2;
    contrl[6] = handle;
    vdi();
}

    WORD
v_bar( handle, xy )
WORD handle, xy[];
{
    i_ptsin( xy );

    contrl[0] = 11;
    contrl[1] = 2;
    contrl[3] = 0;
    contrl[5] = 1;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
v_bit_image( handle, filename, aspect, scaling, num_pts, xy )
WORD handle, aspect, scaling, num_pts;
WORD xy[];
BYTE *filename;
{
    WORD i;

    for (i = 0; i < 4; i++)
	ptsin[i] = xy[i];
    intin[0] = aspect;
    intin[1] = scaling;
    i = 2;
    while (intin[i++] = *filename++)
        ;

    contrl[0] = 5;
    contrl[1] = num_pts;
    contrl[3] = --i;
    contrl[5] = 23;
    contrl[6] = handle;
    vdi();
}

    WORD
v_cellarray( handle, xy, row_length, el_per_row, num_rows, wr_mode, colors )
WORD handle, xy[4], row_length, el_per_row, num_rows, wr_mode, *colors;
{
    i_intin( colors );
    i_ptsin( xy );

    contrl[0] = 10;
    contrl[1] = 2;
    contrl[3] = row_length * num_rows;
    contrl[6] = handle;
    contrl[7] = row_length;
    contrl[8] = el_per_row;
    contrl[9] = num_rows;
    contrl[10] = wr_mode;
    vdi();

    i_intin( intin );
    i_ptsin( ptsin );
}

    WORD
v_circle( handle, xc, yc, rad )
WORD handle, xc, yc, rad;
{
    ptsin[0] = xc;
    ptsin[1] = yc;
    ptsin[2] = 0;
    ptsin[3] = 0;
    ptsin[4] = rad;
    ptsin[5] = 0;

    contrl[0] = 11;
    contrl[1] = 3;
    contrl[3] = 0;
    contrl[5] = 4;
    contrl[6] = handle;
    vdi();
}

    WORD
v_clear_disp_list( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 22;
    contrl[6] = handle;
    vdi();
}

    WORD
v_clrwk( handle )
WORD handle;
{
    contrl[0] = 3;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
}

    WORD
v_clsvwk( handle )
WORD handle;
{
    contrl[0] = 101;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
}

    WORD
v_clswk( handle )
WORD handle;
{
    contrl[0] = 2;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
}

    WORD
v_contourfill( handle, x, y, index )
WORD handle, x, y, index;
{
    intin[0] = index;
    ptsin[0] = x;
    ptsin[1] = y;

    contrl[0] = 103;
    contrl[1] = 1;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
}

    WORD
v_curdown( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 5;
    contrl[6] = handle;
    vdi();
}

    WORD
v_curhome( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 8;
    contrl[6] = handle;
    vdi();
}

    WORD
v_curleft( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 7;
    contrl[6] = handle;
    vdi();
}

    WORD
v_curright( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 6;
    contrl[6] = handle;
    vdi();
}

    WORD
v_curtext( handle, string )
WORD handle;
BYTE *string; 
{
    WORD *intstr;

    intstr = intin;
    while (*intstr++ = *string++)
      ;

    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = ((int)(intstr - intin)-1);
    contrl[5] = 12;
    contrl[6] = handle;
    vdi();
}

    WORD
v_curup( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 4;
    contrl[6] = handle;
    vdi();
}

    WORD
v_dspcur( handle, x, y )
WORD handle, x, y;
{
    ptsin[0] = x;
    ptsin[1] = y;

    contrl[0] = 5;
    contrl[1] = 1;
    contrl[3] = 0;
    contrl[5] = 18;
    contrl[6] = handle;
    vdi();
}

    WORD
v_eeol( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 10;
    contrl[6] = handle;
    vdi();
}

    WORD
v_eeos( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 9;
    contrl[6] = handle;
    vdi();
}

    WORD
v_ellarc( handle, xc, yc, xrad, yrad, sang, eang )
WORD handle, xc, yc, xrad, yrad, sang, eang;
{
    ptsin[0] = xc;
    ptsin[1] = yc;
    ptsin[2] = xrad;
    ptsin[3] = yrad;
    intin[0] = sang;
    intin[1] = eang;

    contrl[0] = 11;
    contrl[1] = 2;
    contrl[3] = 2;
    contrl[5] = 6;
    contrl[6] = handle;
    vdi();
}

    WORD
v_ellipse( handle, xc, yc, xrad, yrad )
WORD handle, xc, yc, xrad, yrad;
{
    ptsin[0] = xc;
    ptsin[1] = yc;
    ptsin[2] = xrad;
    ptsin[3] = yrad;

    contrl[0] = 11;
    contrl[1] = 2;
    contrl[3] = 0;
    contrl[5] = 5;
    contrl[6] = handle;
    vdi();
}

    WORD
v_ellpie( handle, xc, yc, xrad, yrad, sang, eang)
WORD handle, xc, yc, xrad, yrad, sang, eang;
{
    ptsin[0] = xc;
    ptsin[1] = yc;
    ptsin[2] = xrad;
    ptsin[3] = yrad;
    intin[0] = sang;
    intin[1] = eang;

    contrl[0] = 11;
    contrl[1] = 2;
    contrl[3] = 2;
    contrl[5] = 7;
    contrl[6] = handle;
    vdi();
}

    WORD
v_enter_cur( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 3;
    contrl[6] = handle;
    vdi();
}

    WORD
vex_butv( handle, usercode, savecode )
WORD handle;
LONG usercode, *savecode;
{
    i_ptr( usercode );   

    contrl[0] = 125;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    m_lptr2( savecode );
}

    WORD
vex_curv( handle, usercode, savecode )
WORD handle;
LONG usercode, *savecode;
{
    i_ptr( usercode );

    contrl[0] = 127;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    m_lptr2( savecode );
}

    WORD
v_exit_cur( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 2;
    contrl[6] = handle;
    vdi();
}

    WORD
vex_motv( handle, usercode, savecode )
WORD handle;
LONG usercode, *savecode;
{
    i_ptr( usercode );

    contrl[0] = 126;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    m_lptr2( savecode );
}

    WORD
vex_timv( handle, tim_addr, old_addr, scale )
WORD handle, *scale;
LONG tim_addr, *old_addr;
{
    i_ptr( tim_addr );

    contrl[0] = 118;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    m_lptr2( old_addr );
    *scale = intout[0];
}

    WORD
v_fillarea( handle, count, xy)
WORD handle, count, xy[];
{
    i_ptsin( xy );

    contrl[0] = 9;
    contrl[1] = count;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
v_form_adv( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 20;
    contrl[6] = handle;
    vdi();
}

    WORD
v_get_pixel( handle, x, y, pel, index )
WORD handle, x, y, *pel, *index;
{
    ptsin[0] = x;
    ptsin[1] = y;

    contrl[0] = 105;
    contrl[1] = 1;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    *pel = intout[0];
    *index = intout[1];
}

    WORD
v_gtext( handle, x, y, string)
WORD handle, x, y;
BYTE *string;
{
    WORD i;

    ptsin[0] = x;
    ptsin[1] = y;
    i = 0;
    while (intin[i++] = *string++)
        ;

    contrl[0] = 8;
    contrl[1] = 1;
    contrl[3] = --i;
    contrl[6] = handle;
    vdi();
}

    WORD
v_hardcopy( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 17;
    contrl[6] = handle;
    vdi();
}

    WORD
v_hide_c( handle )
WORD handle;
{
    contrl[0] = 123;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
}

    WORD
v_justified( handle, x, y, string, length, word_space, char_space)
WORD handle, x, y, length, word_space, char_space;
BYTE string[];
{
    WORD *intstr;

    ptsin[0] = x;
    ptsin[1] = y;
    ptsin[2] = length;
    ptsin[3] = 0;
    intin[0] = word_space;
    intin[1] = char_space;
    intstr = &intin[2];
    while (*intstr++ = *string++)
        ;

    contrl[0] = 11;
    contrl[1] = 2;
    contrl[3] = (int) (intstr - intin) - 1;
    contrl[5] = 10;
    contrl[6] = handle;
    vdi();
}

    WORD
v_meta_extents( handle, min_x, min_y, max_x, max_y )
WORD handle, min_x, min_y, max_x, max_y;
{
    ptsin[0] = min_x;
    ptsin[1] = min_y;
    ptsin[2] = max_x;
    ptsin[3] = max_y;

    contrl[0] = 5;
    contrl[1] = 2;
    contrl[3] = 0;
    contrl[5] = 98;
    contrl[6] = handle;
    vdi();
}

    WORD
vm_filename( handle, filename )
WORD handle;
BYTE *filename;
{
    WORD *intstr;

    intstr = intin;
    while( *intstr++ = *filename++ )
        ;

    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = ((int)(intstr - intin)-1);
    contrl[5] = 100;
    contrl[6] = handle;
    vdi();
}

    WORD
v_opnvwk( work_in, handle, work_out )
WORD work_in[], *handle, work_out[];
{
   i_intin( work_in );
   i_intout( work_out );
   i_ptsout( work_out + 45 );

   contrl[0] = 100;
   contrl[1] = 0;
   contrl[3] = 11;
   contrl[6] = *handle;
   vdi();

   *handle = contrl[6];    
   i_intin( intin );
   i_intout( intout );
   i_ptsout( ptsout );
   i_ptsin( ptsin );	/* must set in 68k land so we can ROM it */
}

    WORD
v_opnwk( work_in, handle, work_out )
WORD work_in[], *handle, work_out[];
{
   i_intin( work_in );
   i_intout( work_out );
   i_ptsout( work_out + 45 );

   contrl[0] = 1;
   contrl[1] = 0;        /* no points to xform */
   contrl[3] = 11;        /* pass down xform mode also */
   vdi();

   *handle = contrl[6];    

   i_intin( intin );
   i_intout( intout );
   i_ptsout( ptsout );
   i_ptsin( ptsin );	/* must set in 68k land so we can ROM it */
}

    WORD
v_output_window( handle, xy )
WORD handle, xy[];
{
    i_ptsin( xy );

    contrl[0] = 5;
    contrl[1] = 2;
    contrl[3] = 0;
    contrl[5] = 21;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
v_pieslice( handle, xc, yc, rad, sang, eang )
WORD handle, xc, yc, rad, sang, eang;
{
    ptsin[0] = xc;
    ptsin[1] = yc;
    ptsin[2] = 0;
    ptsin[3] = 0;
    ptsin[4] = 0;
    ptsin[5] = 0;
    ptsin[6] = rad;
    ptsin[7] = 0;
    intin[0] = sang;
    intin[1] = eang;

    contrl[0] = 11;
    contrl[1] = 4;
    contrl[3] = 2;
    contrl[5] = 3;
    contrl[6] = handle;
    vdi();
}

    WORD
v_pline( handle, count, xy )
WORD handle, count, xy[];
{
    i_ptsin( xy );

    contrl[0] = 6;
    contrl[1] = count;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
v_pmarker( handle, count, xy )
WORD handle, count, xy[];
{
    i_ptsin( xy );

    contrl[0] = 7;
    contrl[1] = count;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
vq_cellarray( handle, xy, row_len, num_rows, el_used, rows_used, stat, colors )
WORD handle, xy[], row_len, num_rows, *el_used, *rows_used, *stat, colors[];
{
    i_ptsin( xy );
    i_intout( colors );

    contrl[0] = 27;
    contrl[1] = 2;
    contrl[3] = 0;
    contrl[6] = handle;
    contrl[7] = row_len;
    contrl[8] = num_rows;
    vdi();

    *el_used = contrl[9];
    *rows_used = contrl[10];
    *stat = contrl[11];
    i_ptsin( ptsin );
    i_intout( intout );
}

    WORD
vq_chcells( handle, rows, columns )
WORD handle, *rows, *columns;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 1;
    contrl[6] = handle;
    vdi();

    *rows = intout[0];
    *columns = intout[1];
}

    WORD
vq_color( handle, index, set_flag, rgb )
WORD handle, index, set_flag, rgb[];
{
    intin[0] = index;
    intin[1] = set_flag;

    contrl[0] = 26;
    contrl[1] = 0;
    contrl[3] = 2;
    contrl[6] = handle;
    vdi();

    rgb[0] = intout[1];
    rgb[1] = intout[2];
    rgb[2] = intout[3];
}

    WORD
vq_curaddress( handle, row, column )
WORD handle, *row, *column;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 15;
    contrl[6] = handle;
    vdi();

    *row = intout[0];
    *column = intout[1];
}

    WORD
vq_extnd( handle, owflag, work_out )
WORD handle, owflag, work_out[];
{
    i_intin( intin );	/* must set in 68k land so we can ROM it */
    i_ptsin( ptsin );	/* since bss can't have initialized data */

    i_intout( work_out );
    i_ptsout( work_out + 45 );
    intin[0] = owflag;

    contrl[0] = 102;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    i_intout( intout );
    i_ptsout( ptsout );
}

    WORD
vqf_attributes( handle, attributes )
WORD handle, attributes[];
{
    i_intout( attributes );

    contrl[0] = 37;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_intout( intout );
}

    WORD
vqin_mode( handle, dev_type, mode )
WORD handle, dev_type, *mode;
{
    intin[0] = dev_type;

    contrl[0] = 115;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    *mode = intout[0];
}

    WORD
vq_key_s( handle, status )
WORD handle, *status;
{
    contrl[0] = 128;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    *status = intout[0];
}

    WORD
vql_attributes( handle, attributes )
WORD handle, attributes[];
{
    i_intout( attributes );

    contrl[0] = 35;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_intout( intout );
    attributes[3] = ptsout[0];
}

    WORD
vqm_attributes( handle, attributes )
WORD handle, attributes[];
{
    i_intout( attributes );

    contrl[0] = 36;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_intout( intout );
    attributes[3] = ptsout[1];
}

    WORD
vq_mouse( handle, status, px, py )
WORD handle, *status, *px, *py;
{
    contrl[0] = 124;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    *status = intout[0];
    *px = ptsout[0];
    *py = ptsout[1];
}

    WORD
vqp_error( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 96;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vqp_films( handle, names )
WORD handle;
BYTE names[];
{
    WORD   i;

    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 91;
    contrl[6] = handle;
    vdi();

    for (i = 0; i < contrl[4]; i++);
        names[i] = intout[i];
}

    WORD
vqp_state( handle, port, filmnum, lightness, interlace, planes, indexes )
WORD handle, *port, *filmnum, *lightness, *interlace, *planes, *indexes;
{
    WORD i;

    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 92;
    contrl[6] = handle;
    vdi();

    *port = intout[0];
    *filmnum = intout[1];
    *lightness = intout[2];
    *interlace = intout[3];
    *planes = intout[4];
    for (i = 5; i < contrl[4]; i++);
        *indexes++ = intout[i];
}

    WORD
vq_tabstatus( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 16;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vqt_attributes( handle, attributes )
WORD handle, attributes[];
{
    i_intout( attributes );
    i_ptsout( attributes+6 );

    contrl[0] = 38;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_intout( intout );
    i_ptsout( ptsout );
}

    WORD
vqt_extent( handle, string, extent )
WORD handle, extent[];
BYTE string[];
{
    WORD *intstr;

    intstr = intin;
    while (*intstr++ = *string++)
        ;
    i_ptsout( extent );

    contrl[0] = 116;
    contrl[1] = 0;
    contrl[3] = ((int)(intstr - intin)-1);
    contrl[6] = handle;
    vdi();

    i_ptsout( ptsout );
}

    WORD
vqt_font_info( handle, minADE, maxADE, distances, maxwidth, effects )
WORD handle, *minADE, *maxADE, distances[], *maxwidth, effects[];
{
    contrl[0] = 131;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    *minADE = intout[0];
    *maxADE = intout[1];
    *maxwidth = ptsout[0];
    distances[0] = ptsout[1];
    distances[1] = ptsout[3];
    distances[2] = ptsout[5];
    distances[3] = ptsout[7];
    distances[4] = ptsout[9];
    effects[0] = ptsout[2];
    effects[1] = ptsout[4];
    effects[2] = ptsout[6];
}

    WORD
vqt_name( handle, element_num, name )
WORD handle, element_num;
BYTE name[];
{
    WORD i;

    intin[0] = element_num;

    contrl[0] = 130;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    for (i = 0 ; i < 32 ; i++)
	name[i] = intout[i + 1];
    return( intout[0] );
}

    WORD
vqt_width( handle, character, cell_width, left_delta, right_delta )
WORD handle, *cell_width, *left_delta, *right_delta;
BYTE character;
{
    intin[0] = character;

    contrl[0] = 117;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    *cell_width = ptsout[0];
    *left_delta = ptsout[2];
    *right_delta = ptsout[4];
    return( intout[0] );
}

    WORD
v_rbox( handle, xy )
WORD handle, xy[];
{
    i_ptsin( xy );

    contrl[0] = 11;
    contrl[1] = 2;
    contrl[3] = 0;
    contrl[5] = 8;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
v_rfbox( handle, xy )
WORD handle, xy[];
{
    i_ptsin( xy );

    contrl[0] = 11;
    contrl[1] = 2;
    contrl[3] = 0;
    contrl[5] = 9;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
v_rmcur( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 19;
    contrl[6] = handle;
    vdi();
}

    WORD
vro_cpyfm( handle, wr_mode, xy, srcMFDB, desMFDB )
WORD handle, wr_mode, xy[], *srcMFDB, *desMFDB;
{
    intin[0] = wr_mode;
    i_ptr( srcMFDB );
    i_ptr2( desMFDB );
    i_ptsin( xy );

    contrl[0] = 109;
    contrl[1] = 4;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
vrq_choice( handle, in_choice, out_choice )
WORD handle, in_choice, *out_choice;
{
    intin[0] = in_choice;

    contrl[0] = 30;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    *out_choice = intout[0];
}

    WORD
vrq_locator( handle, initx, inity, xout, yout, term )
WORD handle, initx, inity, *xout, *yout, *term;
{
    ptsin[0] = initx;
    ptsin[1] = inity;

    contrl[0] = 28;
    contrl[1] = 1;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    *xout = ptsout[0];
    *yout = ptsout[1];
    *term = intout[0];
}

    WORD
vrq_string( handle, length, echo_mode, echo_xy, string)
WORD handle, length, echo_mode, echo_xy[];
BYTE *string;
{
    WORD    count;

    intin[0] = length;
    intin[1] = echo_mode;
    i_ptsin( echo_xy );

    contrl[0] = 31;
    contrl[1] = echo_mode;
    contrl[3] = 2;
    contrl[6] = handle;
    vdi();

    for (count = 0; count < contrl[4]; count++)
      *string++ = intout[count];
    *string = 0;  
    i_ptsin( ptsin );
}

    WORD
vrq_valuator( handle, val_in, val_out, term )
WORD handle, val_in, *val_out, *term;
{
    intin[0] = val_in;

    contrl[0] = 29;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    *val_out = intout[0];
    *term = intout[1];
}

    WORD
vr_recfl( handle, xy )
WORD handle, *xy;
{
    i_ptsin( xy );

    contrl[0] = 114;
    contrl[1] = 2;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
vrt_cpyfm( handle, wr_mode, xy, srcMFDB, desMFDB, index )
WORD handle, wr_mode, *srcMFDB, *desMFDB, xy[], *index;
{
    intin[0] = wr_mode;
    intin[1] = *index++;
    intin[2] = *index;
    i_ptr( srcMFDB );
    i_ptr2( desMFDB );
    i_ptsin( xy );

    contrl[0] = 121;
    contrl[1] = 4;
    contrl[3] = 3;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
vr_trnfm( handle, srcMFDB, desMFDB )
WORD handle, *srcMFDB, *desMFDB;
{
    i_ptr( srcMFDB );
    i_ptr2( desMFDB );

    contrl[0] = 110;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
}

    WORD
v_rvoff( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 14;
    contrl[6] = handle;
    vdi();
}

    WORD
v_rvon( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 13;
    contrl[6] = handle;
    vdi();
}

    WORD
vsc_form( handle, cur_form )
WORD handle, *cur_form;
{
    i_intin( cur_form );

    contrl[0] = 111;
    contrl[1] = 0;
    contrl[3] = 37;
    contrl[6] = handle;
    vdi();

    i_intin( intin );
}

    WORD
vs_clip( handle, clip_flag, xy )
WORD handle, clip_flag, xy[];
{
    i_ptsin( xy );
    intin[0] = clip_flag;

    contrl[0] = 129;
    contrl[1] = 2;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    i_ptsin( ptsin );
}

    WORD
vs_color( handle, index, rgb )
WORD handle, index, *rgb;
{
    WORD i;

    intin[0] = index;
    for ( i = 1; i < 4; i++ )
        intin[i] = *rgb++;

    contrl[0] = 14;
    contrl[1] = 0;
    contrl[3] = 4;
    contrl[6] = handle;
    vdi();
}

    WORD
vs_curaddress( handle, row, column )
WORD handle, row, column;
{
    intin[0] = row;
    intin[1] = column;

    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 2;
    contrl[5] = 11;
    contrl[6] = handle;
    vdi();
}

    WORD
vsf_color( handle, index )
WORD handle, index;
{
    intin[0] = index;

    contrl[0] = 25;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}
    
    WORD
vsf_interior( handle, style )
WORD handle, style;
{
    intin[0] = style;

    contrl[0] = 23;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsf_perimeter( handle, per_vis )
WORD handle, per_vis;
{
    intin[0] = per_vis;

    contrl[0] = 104;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsf_style( handle, index )
WORD handle, index;
{
    intin[0] = index;

    contrl[0] = 24;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsf_udpat( handle, fill_pat, planes )
WORD handle, fill_pat[], planes;
{
    i_intin( fill_pat );

    contrl[0] = 112;
    contrl[1] = 0;
    contrl[3] = 16*planes;
    contrl[6] = handle;
    vdi();
    i_intin( intin );
}

    WORD
v_show_c( handle, reset )
WORD handle, reset;
{
    intin[0] = reset;

    contrl[0] = 122;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
}

    WORD
vsin_mode( handle, dev_type, mode )
WORD handle, dev_type, mode;
{
    intin[0] = dev_type;
    intin[1] = mode;

    contrl[0] = 33;
    contrl[1] = 0;
    contrl[3] = 2;
    contrl[6] = handle;
    vdi();
}

    WORD
vsl_color( handle, index )
WORD handle, index;
{
    intin[0] = index;

    contrl[0] = 17;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsl_ends( handle, beg_style, end_style)
WORD handle, beg_style, end_style;
{
    intin[0] = beg_style;
    intin[1] = end_style;

    contrl[0] = 108;
    contrl[1] = 0;
    contrl[3] = 2;
    contrl[6] = handle;
    vdi();
}

    WORD
vsl_type( handle, style )
WORD handle, style;
{
    intin[0] = style;

    contrl[0] = 15;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsl_udsty( handle, pattern )
WORD handle, pattern;
{
    intin[0] = pattern;

    contrl[0] = 113;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
}

    WORD
vsl_width( handle, width )
WORD handle, width;
{
    ptsin[0] = width;
    ptsin[1] = 0;

    contrl[0] = 16;
    contrl[1] = 1;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
    return( ptsout[0] );
}

    WORD
vsm_choice( handle, choice )
WORD handle, *choice;
{
    contrl[0] = 30;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    *choice = intout[0];
    return( contrl[4] );
}

    WORD
vsm_color( handle, index )
WORD handle, index;
{
    intin[0] = index;

    contrl[0] = 20;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsm_height( handle, height )
WORD handle, height;
{
    ptsin[0] = 0;
    ptsin[1] = height;

    contrl[0] = 19;
    contrl[1] = 1;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
    return( ptsout[1] );
}

    WORD
vsm_locator( handle, initx, inity, xout, yout, term )
WORD handle, initx, inity, *xout, *yout, *term;
{
    ptsin[0] = initx;
    ptsin[1] = inity;

    contrl[0] = 28;
    contrl[1] = 1;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    *xout = ptsout[0];
    *yout = ptsout[1];
    *term = intout[0];
    return( (contrl[4] << 1) | contrl[2] );
}

    WORD
vsm_string( handle, length, echo_mode, echo_xy, string )
WORD handle, length, echo_mode, echo_xy[];
BYTE *string;
{
    WORD    count;

    intin[0] = length;
    intin[1] = echo_mode;
    i_ptsin( echo_xy );

    contrl[0] = 31;
    contrl[1] = echo_mode;
    contrl[3] = 2;
    contrl[6] = handle;
    vdi();

    for (count = 0; count < contrl[4]; count++)
      *string++ = intout[count];
    *string = 0;  
    i_ptsin( ptsin );
    return( contrl[4] );
}

    WORD
vsm_type( handle, symbol )
WORD handle, symbol;
{
    intin[0] = symbol;

    contrl[0] = 18;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vsm_valuator( handle, val_in, val_out, term, status )
WORD handle, val_in, *val_out, *term, *status;
{
    intin[0] = val_in;

    contrl[0] = 29;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    *val_out = intout[0];
    *term = intout[1];
    *status = contrl[4];
}

    WORD
vs_palette( handle, palette )
WORD handle, palette;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[5] = 60;
    contrl[6] = handle;
    intin[0] = palette;
    vdi();
    return( intout[0] );
}

    WORD
vsp_message( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 95;
    contrl[6] = handle;
    vdi();
}

    WORD
vsp_save( handle )
WORD handle;
{
    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[5] = 94;
    contrl[6] = handle;
    vdi();
}

    WORD
vsp_state( handle, port, filmnum, lightness, interlace, planes, indexes )
WORD handle, port, filmnum, lightness, interlace, planes, *indexes;
{
    WORD i;

    intin[0] = port; 
    intin[1] = filmnum; 
    intin[2] = lightness;
    intin[3] = interlace;
    intin[4] = planes;
    for (i = 5; i < 20; i++);
        intin[i] = *indexes++;

    contrl[0] = 5;
    contrl[1] = 0;
    contrl[3] = 20;
    contrl[5] = 93;
    contrl[6] = handle;
    vdi();
}

    WORD
vst_alignment( handle, hor_in, vert_in, hor_out, vert_out )
WORD handle, hor_in, vert_in, *hor_out, *vert_out;
{
    intin[0] = hor_in;
    intin[1] = vert_in;

    contrl[0] = 39;
    contrl[1] = 0;
    contrl[3] = 2;
    contrl[6] = handle;
    vdi();

    *hor_out = intout[0];
    *vert_out = intout[1];
}

    WORD
vst_color( handle, index )
WORD handle, index;
{
    intin[0] = index;

    contrl[0] = 22;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vst_effects( handle, effect )
WORD handle, effect;
{
    intin[0] = effect;

    contrl[0] = 106;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vst_font( handle, font )
WORD handle, font;
{
    intin[0] = font;

    contrl[0] = 21;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vst_height( handle, height, char_width, char_height, cell_width, cell_height )
WORD handle, height, *char_width, *char_height, *cell_width, *cell_height;
{
    ptsin[0] = 0;
    ptsin[1] = height;

    contrl[0] = 12;
    contrl[1] = 1;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();

    *char_width = ptsout[0];
    *char_height = ptsout[1];
    *cell_width = ptsout[2];
    *cell_height = ptsout[3];
}

    WORD
vst_load_fonts( handle, select )
WORD handle, select;
{
    contrl[0] = 119;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vst_point( handle, point, char_width, char_height, cell_width, cell_height )
WORD handle, point, *char_width, *char_height, *cell_width, *cell_height;
{
    intin[0] = point;

    contrl[0] = 107;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();

    *char_width = ptsout[0];
    *char_height = ptsout[1];
    *cell_width = ptsout[2];
    *cell_height = ptsout[3];
    return( intout[0] );
}

    WORD
vst_rotation( handle, angle )
WORD handle, angle;
{
    intin[0] = angle;

    contrl[0] = 13;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
vst_unload_fonts( handle, select )
WORD handle, select;
{
    contrl[0] = 120;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
}

    WORD
vswr_mode( handle, mode )
WORD handle, mode;
{
    intin[0] = mode;

    contrl[0] = 32;
    contrl[1] = 0;
    contrl[3] = 1;
    contrl[6] = handle;
    vdi();
    return( intout[0] );
}

    WORD
v_updwk( handle )
WORD handle;
{
    contrl[0] = 4;
    contrl[1] = 0;
    contrl[3] = 0;
    contrl[6] = handle;
    vdi();
}

    WORD
v_write_meta( handle, num_ints, ints, num_pts, pts )
WORD handle, num_ints, ints[], num_pts, pts[];
{
    i_intin( ints );
    i_ptsin( pts );

    contrl[0] = 5;
    contrl[1] = num_pts;
    contrl[3] = num_ints;
    contrl[5] = 99;
    contrl[6] = handle;
    vdi();

    i_intin( intin );
    i_ptsin( ptsin );
}

