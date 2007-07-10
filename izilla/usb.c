//USB.c, used with iZilla

#define outl(a, b) (*(volatile unsigned int *)(b) = (a))
#define inl(a) (*(volatile unsigned int *)(a))

static int usb_connected=1;

static int usb_check_connection()
{
	setup_usb(0);
	
    
    if ((inl(0xc50001A4) & 0x800)==0)
	{
		
		return 0;
	}
	else
	{
		
		return 1;  // we're connected - yay!
	}
}


static int usb_get_usb2d_ident_reg()
{
	return inl(0xc5000000);
}

static int setup_usb(int arg0)
{
	int r0, r1;
	static int usb_inited = 0;

	if (!usb_inited)
	{
		outl(inl(0x70000084) | 0x200, 0x70000084);
		if (arg0==0)
		{
			outl(inl(0x70000080) & ~0x2000, 0x70000080);
			return 0;
		} 
	
		outl(inl(0x7000002C) | 0x3000000, 0x7000002C);
		outl(inl(0x6000600C) | 0x400000, 0x6000600C);
	
		outl(inl(0x60006004) | 0x400000, 0x60006004);   // reset usb start
		outl(inl(0x60006004) & ~0x400000, 0x60006004);  // reset usb end

		outl(inl(0x70000020) | 0x80000000, 0x70000020);

		while ((inl(0x70000028) & 0x80) == 0);
	
		outl(inl(0xc5000184) | 0x100, 0xc5000184);


		while ((inl(0xc5000184) & 0x100) != 0);


		outl(inl(0xc50001A4) | 0x5F000000, 0xc50001A4);

		if ((inl(0xc50001A4) & 0x100) == 0)
		{
			outl(inl(0xc50001A8) & ~0x3, 0xc50001A8);
			outl(inl(0xc50001A8) | 0x2, 0xc50001A8);
 			outl(inl(0x70000028) | 0x4000, 0x70000028);
			outl(inl(0x70000028) | 0x2, 0x70000028);
		} else {

			outl(inl(0xc50001A8) | 0x3, 0xc50001A8);
			outl(inl(0x70000028) &~0x4000, 0x70000028);
			outl(inl(0x70000028) | 0x2, 0x70000028);
		}
		outl(inl(0xc5000140) | 0x2, 0xc5000140);
		while((inl(0xc5000140) & 0x2) != 0);
		r0 = inl(0xc5000184);
	///////////////THIS NEEDS TO BE CHANGED ONCE THERE IS KERNEL USB
		outl(inl(0x70000020) | 0x80000000, 0x70000020);
		outl(inl(0x6000600C) | 0x400000, 0x6000600C);
		while ((inl(0x70000028) & 0x80) == 0);
		outl(inl(0x70000028) | 0x2, 0x70000028);
	
		//wait_usec(0x186A0);
		usleep(0x186A0);	
	/////////////////////////////////////////////////////////////////
		r0 = r0 << 4;
		r1 = r0 >> 30;
		usb_inited = 1;	
		return r1; // if r1>2 or r1 < 0 then it returns crap - not sure what
	} else { 
		return 0;
	}
}



static int usb_test_core(int arg0)
{
	int r0;
	setup_usb(1);
	r0 = usb_get_usb2d_ident_reg();
	if (r0!=arg0)
	{
		///// USB2D_IDENT iS BAD
		return 0;
	} else {

		if (!usb_check_connection())
		{
			/// we're not connected to anything
			return 0;	
		} else {
		
			return 1;	
			// display id from r0
		}	
	}
}



int usb_is_connected(void)
{

/*	if (hw_version >= 0x4)
	{	*/
		static int r0;
		r0 = usb_test_core(0x22fA05);
		return r0;
	//} 

	return 0;
}


static void goto_diskmode()
{

	unsigned char * storage_ptr = (unsigned char *)0x40017F00;
	char * diskmode = "diskmode\0";
	char * hotstuff = "hotstuff\0";

	memcpy(storage_ptr, diskmode, 9);
	storage_ptr = (unsigned char *)0x40017f08;
	memcpy(storage_ptr, hotstuff, 9);
	outl(1, 0x40017F10);	
	outl(inl(0x60006004) | 0x4, 0x60006004);

}
