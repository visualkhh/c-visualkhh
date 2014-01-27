<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' version='1.0'>
	
	<xsl:template match='/'>
			<html>
				<head>
					<title>aa</title>
				</head>	
				
				<body>
					s
					<xsl:apply-templates select="root"/>
					s
				</body>
				
			</html>
			
	</xsl:template>
	
	
	<xsl:template match="root">
		<xsl:value-of select="port/p/a[1]"/>
	</xsl:template>

</xsl:stylesheet>
