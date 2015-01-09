#include "GraphicsSupport.h"

namespace org {
    namespace antlr {
        namespace v4 {
            namespace runtime {
                namespace misc {

                    void GraphicsSupport::saveImage(JComponent *const comp, const std::wstring &fileName) throw(IOException, PrintException) {
//JAVA TO C++ CONVERTER TODO TASK: There is no direct native C++ equivalent to the Java String 'endsWith' method:
                        if (fileName.endsWith(L".ps") || fileName.endsWith(L".eps")) {
                            DocFlavor *flavor = DocFlavor::SERVICE_FORMATTED::PRINTABLE;
                            std::wstring mimeType = L"application/postscript";
//JAVA TO C++ CONVERTER WARNING: Since the array size is not known in this declaration, Java to C++ Converter has converted this array to a pointer.  You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: javax.print.StreamPrintServiceFactory[] factories = javax.print.StreamPrintServiceFactory.lookupStreamPrintServiceFactories(flavor, mimeType);
                            StreamPrintServiceFactory *factories = StreamPrintServiceFactory::lookupStreamPrintServiceFactories(flavor, mimeType);
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
                            std::cout << Arrays->toString(factories) << std::endl;
                            if (factories->length > 0) {
                                FileOutputStream *out = new FileOutputStream(fileName);
                                PrintService *service = factories[0]->getPrintService(out);
                                SimpleDoc *doc = new SimpleDoc(new PrintableAnonymousInnerClassHelper(comp), flavor, nullptr);
                                DocPrintJob *job = service->createPrintJob();
                                PrintRequestAttributeSet *attributes = new HashPrintRequestAttributeSet();
                                job->print(doc, attributes);
                                out->close();
                            }
                        } else {
                            // parrt: works with [image/jpeg, image/png, image/x-png, image/vnd.wap.wbmp, image/bmp, image/gif]
                            Rectangle *rect = comp->getBounds();
                            BufferedImage *image = new BufferedImage(rect->width, rect->height, BufferedImage::TYPE_INT_RGB);
                            Graphics2D *g = static_cast<Graphics2D*>(image->getGraphics());
                            g->setColor(Color::WHITE);
                            g->fill(rect);
                                        //			g.setColor(Color.BLACK);
                            comp->paint(g);
                            std::wstring extension = fileName.substr(fileName.rfind(L'.') + 1);
                            bool result = ImageIO::write(image, extension, new File(fileName));
                            if (!result) {
                                System::err::println(std::wstring(L"Now imager for ") + extension);
                            }
                            g->dispose();
                        }
                    }

                    GraphicsSupport::PrintableAnonymousInnerClassHelper::PrintableAnonymousInnerClassHelper(JComponent *comp) {
                        this->comp = comp;
                    }

                    int GraphicsSupport::PrintableAnonymousInnerClassHelper::print(Graphics *g, PageFormat *pf, int page) {
                        if (page >= 1) {
                            return Printable::NO_SUCH_PAGE;
                        } else {
                            Graphics2D *g2 = static_cast<Graphics2D*>(g);
                            g2->translate((pf->getWidth() - pf->getImageableWidth()) / 2, (pf->getHeight() - pf->getImageableHeight()) / 2);
                            if (comp->getWidth() > pf->getImageableWidth() || comp->getHeight() > pf->getImageableHeight()) {
                                double sf1 = pf->getImageableWidth() / (comp->getWidth() + 1);
                                double sf2 = pf->getImageableHeight() / (comp->getHeight() + 1);
                                double s = std::min(sf1, sf2);
                                g2->scale(s, s);
                            }

                            comp->paint(g);
                            return Printable::PAGE_EXISTS;
                        }
                    }
                }
            }
        }
    }
}
