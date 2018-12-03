# STM32_ZhoneKePower
ChargingMachine/C language/Arm cortex-M

//验证8583数据正确性
		int msgType = -1;
		long msgId = -1;
		byte[] mapData = null;
		My8583 map = null;
		Integer action = null;
		if (data[0] == (byte)0xA5) {
			check = 0;
			int len = (data[1] < 0 ? 256 + data[1] : data[1]) * 256 + (data[2] < 0 ? 256 + data[2] : data[2]);
			for (int i=0; i<len; i++) {
				check ^= data[1 + i];
			}
			if (check != 0)	return;
			
			msgType = (data[3] < 0 ? 256 + data[3] : data[3])  * 256 + (data[4] < 0 ? 256 + data[4] : data[4]);
			msgId = (data[5] < 0 ? 256 + data[5] : data[5]) * 16777216L
					+ (data[6] < 0 ? 256 + data[6] : data[6]) * 65536
					+ (data[7] < 0 ? 256 + data[7] : data[7]) * 256
					+ (data[8] < 0 ? 256 + data[8] : data[8]);
			mapData = Arrays.copyOfRange(data, 9, len);
			System.out.println(JavaUtil.toHexString(mapData));
			map = My8583.create8583(mapData);        //解析位图跟DATA
			if (map == null)	return;
			
			action = map.getInteger(0);	//0位为action
			if (action == null)	return;
		} else {
			return;
	}
	}
