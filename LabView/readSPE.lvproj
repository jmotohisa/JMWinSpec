<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="20008000">
	<Property Name="NI.LV.All.SourceOnly" Type="Bool">true</Property>
	<Item Name="マイ コンピュータ" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">マイ コンピュータ/VIサーバ</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">マイ コンピュータ/VIサーバ</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="double_binread.vi" Type="VI" URL="../double_binread.vi"/>
		<Item Name="float_binread.vi" Type="VI" URL="../float_binread.vi"/>
		<Item Name="int_binread.vi" Type="VI" URL="../int_binread.vi"/>
		<Item Name="long_binread.vi" Type="VI" URL="../long_binread.vi"/>
		<Item Name="Read and Write Binary File Examples.vi" Type="VI" URL="../Read and Write Binary File Examples.vi"/>
		<Item Name="read_exptime.vi" Type="VI" URL="../read_exptime.vi"/>
		<Item Name="readSPEHeader.vi" Type="VI" URL="../readSPEHeader.vi"/>
		<Item Name="readSPEHeaderTest.vi" Type="VI" URL="../readSPEHeaderTest.vi"/>
		<Item Name="short_binread.vi" Type="VI" URL="../short_binread.vi"/>
		<Item Name="依存項目" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="compatCalcOffset.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatCalcOffset.vi"/>
				<Item Name="compatFileDialog.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatFileDialog.vi"/>
				<Item Name="compatOpenFileOperation.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatOpenFileOperation.vi"/>
				<Item Name="Open_Create_Replace File.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/Open_Create_Replace File.vi"/>
			</Item>
		</Item>
		<Item Name="ビルド仕様" Type="Build"/>
	</Item>
</Project>
