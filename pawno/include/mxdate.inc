/*
	���������� UNIX TIMESTAMP � ������������ � ��������� �����.

	���������
		UNIX TIMESTAMP ��� ���-�� ������ �� ��� 01.01.1970, 00:00:00 ��
		��������� � ���������� ����

	������
		����� integer (4 �����)
*/
stock date_to_timestamp
(
	mx_year = 1970,	mx_month  = 1,	mx_day    = 1,
	mx_hour =    0,	mx_minute = 0,	mx_second = 0
)
{
	switch ( mx_month )
	{
		case  2 : mx_day +=  31;	case  3 : mx_day +=  59;	case  4 : mx_day +=  90;
		case  5 : mx_day += 120;	case  6 : mx_day += 151;	case  7 : mx_day += 181;
		case  8 : mx_day += 212;	case  9 : mx_day += 243;	case 10 : mx_day += 273;
		case 11 : mx_day += 304;	case 12 : mx_day += 334;
	}

	return ( mx_second + mx_minute*60 + mx_hour*3600 + (mx_day - 1)*86400
		+ (mx_year - 1970)*31557600 + ( mx_year % 4 == 0 ? 21600 : 0 ) );
}


/*
	���������� �� ������ ���� � ������������ � ��������� UNIX TIMESTAMP.

	���������
		UNIX TIMESTAMP ��� ���-�� ������ �� ��� 01.01.1970, 00:00:00 ��
		��������� � ���������� ����. ���� �� ��������� ������, �� �
		mx_year ����� ������� ���, � mx_month - �����, � mx_day - ����,
		� mx_hour - ���, � mx_minute - ������, � mx_second - �������.

	������
		������
*/
stock timestamp_to_date
(
	unix_timestamp = 0,

	& mx_year = 1970,		& mx_month  = 1,		& mx_day    = 1,
	& mx_hour =    0,		& mx_minute = 0,		& mx_second = 0
)
{
	mx_year = unix_timestamp / 31557600;
	unix_timestamp -= mx_year * 31557600;
	mx_year += 1970;

	if ( mx_year % 4 == 0 ) unix_timestamp -= 21600;

	mx_day = unix_timestamp / 86400;

	switch ( mx_day )
	{
		// � mx_second ����� �������� �������� ����� ������
		case    0..30 : { mx_second = mx_day;       mx_month =  1; }
		case   31..58 : { mx_second = mx_day -  31; mx_month =  2; }
		case   59..89 : { mx_second = mx_day -  59; mx_month =  3; }
		case  90..119 : { mx_second = mx_day -  90; mx_month =  4; }
		case 120..150 : { mx_second = mx_day - 120; mx_month =  5; }
		case 151..180 : { mx_second = mx_day - 151; mx_month =  6; }
		case 181..211 : { mx_second = mx_day - 181; mx_month =  7; }
		case 212..242 : { mx_second = mx_day - 212; mx_month =  8; }
		case 243..272 : { mx_second = mx_day - 243; mx_month =  9; }
		case 273..303 : { mx_second = mx_day - 273; mx_month = 10; }
		case 304..333 : { mx_second = mx_day - 304; mx_month = 11; }
		case 334..366 : { mx_second = mx_day - 334; mx_month = 12; }
	}

	unix_timestamp -= mx_day * 86400;
	mx_hour = unix_timestamp / 3600;

	unix_timestamp -= mx_hour * 3600;
	mx_minute = unix_timestamp / 60;

	unix_timestamp -= mx_minute * 60;
	mx_day = mx_second + 1;
	mx_second = unix_timestamp;
}

/*
	�������� � ��������� ������ �������� ����������� ������� �� �� ��������
	� ������������ � ��������� UNIX TIMESTAMP

	���������
		UNIX TIMESTAMP ��� ���-�� ������ �� ��� 01.01.1970, 00:00:00 ��
		��������� � ���������� ����. ���� �� ��������� ������, �� �
		������ format ��� �������� ����������� ������� ����� ��������
		�� �� �������� � ������������ � ��������� UNIX TIMESTAMP.

		������� ����:	�������� ��:
		%yyyy			�������������� �������� ����
		%yy			���������� �������� ���� � ������� ����
		%m			�������� ������
		%mm			�������� ������ � ������� ����
		%d			�������� ���
		%dd			�������� ��� � ������� ����
		%h			�������� ����
		%hh			�������� ���� � ������� ����
		%i			�������� ������
		%ii			�������� ������ � ������� ����
		%s			�������� �������
		%ss			�������� ������� � ������� ����

	������
		������ � ���������� ���������� ���� ������ �� �����������
		������ ������ = sizeOfOutput
*/
stock date ( formatStr[] = "%dd.%mm.%yyyy, %hh:%ii:%ss", timestamp = 0 )
{
	// ------------
	const sizeOfOutput = 128; // ������ ������������ ������

	new yyyy, mm, dd, h, m, s,
		pos, foundPos, searchStartPos, outStrLen,
		tmpNumStr[5], outStr[sizeOfOutput];

	timestamp_to_date( timestamp+36000, yyyy,mm,dd, h,m,s ); // ��������� ����� � ����
	// ------------


	// -------------------
	// ��������� formatStr � outStr
	memcpy( outStr, formatStr, 0, (sizeOfOutput - 1)*4 );
	outStr[sizeOfOutput - 1] = 0; // ���������� ����� ������ � ����� ������������

	outStrLen = strlen(outStr);
	// -------------------


	// mx_year
	// -------------------
	searchStartPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%yyyy", false, searchStartPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 5, "%04d", yyyy ); // tmpNumStr = ������� �������� yyyy

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 4; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		for ( pos += foundPos; pos < outStrLen; pos++ )
			outStr[pos] = outStr[pos + 1];

		outStr[pos] = 0; // ���������� ����� ������
		outStrLen = strlen(outStr); // ������ ������ ���������
		searchStartPos = foundPos + 4; // ������� ������ ����. ������ += 4

		if ( searchStartPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%yyyy", false, searchStartPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// -------------------
	searchStartPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%yy", false, searchStartPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 5, "%04d", yyyy ); // tmpNumStr = ������� �������� yyyy

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos + 2];

		// ����� ����� �� 1 ����������� �������� ������
		for ( pos += foundPos; pos <= outStrLen; pos++ )
			outStr[pos] = outStr[pos + 1];

		outStr[pos] = 0; // ���������� ����� ������
		outStrLen = strlen(outStr); // ������ ������ ���������
		searchStartPos = foundPos + 2; // ������� ������ ����. ������ ����������

		if ( searchStartPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%yy", false, searchStartPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// mx_month
	// -------------------
	foundPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%mm", false, foundPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 3, "%02d", mm ); // tmpNumStr = ������� �������� �����

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		for ( pos += foundPos; pos <= outStrLen; pos++ )
			outStr[pos] = outStr[pos + 1];

		outStr[pos] = 0; // ���������� ����� ������
		outStrLen = strlen(outStr); // ������ ������ ���������
		foundPos += 2; // ������� ������ ����. ������ ����������

		if ( foundPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%mm", false, foundPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// -------------------
	foundPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%m", false, foundPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 3, "%d", mm ); // tmpNumStr = ������� �������� �����

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		if ( mm < 10 )
		{
			for ( pos += foundPos; pos <= outStrLen; pos++ )
				outStr[pos] = outStr[pos + 1];

			outStr[pos] = 0; // ���������� ����� ������
			outStrLen = strlen(outStr); // ������ ������ ���������
			foundPos++; // ������� ������ ����. ������ ����������
		}
		else
			foundPos += 2; // ������� ������ ����. ������ ����������

		if ( foundPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%m", false, foundPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// mx_day
	// -------------------
	foundPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%dd", false, foundPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 3, "%02d", dd ); // tmpNumStr = ������� �������� �����

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		for ( pos += foundPos; pos <= outStrLen; pos++ )
			outStr[pos] = outStr[pos + 1];

		outStr[pos] = 0; // ���������� ����� ������
		outStrLen = strlen(outStr); // ������ ������ ���������
		foundPos += 2; // ������� ������ ����. ������ ����������

		if ( foundPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%dd", false, foundPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// -------------------
	foundPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%d", false, foundPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 3, "%d", dd ); // tmpNumStr = ������� �������� �����

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		if ( dd < 10 )
		{
			for ( pos += foundPos; pos <= outStrLen; pos++ )
				outStr[pos] = outStr[pos + 1];

			outStr[pos] = 0; // ���������� ����� ������
			outStrLen = strlen(outStr); // ������ ������ ���������
			foundPos++; // ������� ������ ����. ������ ����������
		}
		else
			foundPos += 2; // ������� ������ ����. ������ ����������

		if ( foundPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%d", false, foundPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// mx_hour
	// -------------------
	foundPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%hh", false, foundPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 3, "%02d", h ); // tmpNumStr = ������� �������� �����

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		for ( pos += foundPos; pos <= outStrLen; pos++ )
			outStr[pos] = outStr[pos + 1];

		outStr[pos] = 0; // ���������� ����� ������
		outStrLen = strlen(outStr); // ������ ������ ���������
		foundPos += 2; // ������� ������ ����. ������ ����������

		if ( foundPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%hh", false, foundPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// -------------------
	foundPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%h", false, foundPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 3, "%d", h ); // tmpNumStr = ������� �������� �����

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		if ( h < 10 )
		{
			for ( pos += foundPos; pos <= outStrLen; pos++ )
				outStr[pos] = outStr[pos + 1];

			outStr[pos] = 0; // ���������� ����� ������
			outStrLen = strlen(outStr); // ������ ������ ���������
			foundPos++; // ������� ������ ����. ������ ����������
		}
		else
			foundPos += 2; // ������� ������ ����. ������ ����������

		if ( foundPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%h", false, foundPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// mx_minute
	// -------------------
	foundPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%ii", false, foundPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 3, "%02d", m ); // tmpNumStr = ������� �������� �����

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		for ( pos += foundPos; pos <= outStrLen; pos++ )
			outStr[pos] = outStr[pos + 1];

		outStr[pos] = 0; // ���������� ����� ������
		outStrLen = strlen(outStr); // ������ ������ ���������
		foundPos += 2; // ������� ������ ����. ������ ����������

		if ( foundPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%ii", false, foundPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// -------------------
	foundPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%i", false, foundPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 3, "%d", m ); // tmpNumStr = ������� �������� �����

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		if ( m < 10 )
		{
			for ( pos += foundPos; pos <= outStrLen; pos++ )
				outStr[pos] = outStr[pos + 1];

			outStr[pos] = 0; // ���������� ����� ������
			outStrLen = strlen(outStr); // ������ ������ ���������
			foundPos++; // ������� ������ ����. ������ ����������
		}
		else
			foundPos += 2; // ������� ������ ����. ������ ����������

		if ( foundPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%i", false, foundPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// mx_second
	// -------------------
	foundPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%ss", false, foundPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 3, "%02d", s ); // tmpNumStr = ������� �������� �����

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		for ( pos += foundPos; pos <= outStrLen; pos++ )
			outStr[pos] = outStr[pos + 1];

		outStr[pos] = 0; // ���������� ����� ������
		outStrLen = strlen(outStr); // ������ ������ ���������
		foundPos += 2; // ������� ������ ����. ������ ����������

		if ( foundPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%ss", false, foundPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	// -------------------
	foundPos = 0; // ��� ������ ������ = 0
	foundPos = strfind( outStr, "%s", false, foundPos ); // ���� ������ �������

	while ( foundPos != -1 )
	{
		format( tmpNumStr, 3, "%d", s ); // tmpNumStr = ������� �������� �����

		for ( pos = 0; tmpNumStr[pos] != 0 && pos < 2; pos++ ) // ������ ���������� �� ��������
			outStr[foundPos + pos] = tmpNumStr[pos];

		// ����� ����� �� 1 ����������� �������� ������
		if ( s < 10 )
		{
			for ( pos += foundPos; pos <= outStrLen; pos++ )
				outStr[pos] = outStr[pos + 1];

			outStr[pos] = 0; // ���������� ����� ������
			outStrLen = strlen(outStr); // ������ ������ ���������
			foundPos++; // ������� ������ ����. ������ ����������
		}
		else
			foundPos += 2; // ������� ������ ����. ������ ����������

		if ( foundPos < outStrLen ) // ���� ��� ������ ������ ������ ����� ������
			foundPos = strfind( outStr, "%s", false, foundPos ); // ��������� �����
		else break; // ����� - ��������� ����� � ������
	}
	// -------------------


	return outStr;
}
