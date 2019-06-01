<script async src="https://busuanzi.ibruce.info/busuanzi/2.3/busuanzi.pure.mini.js"></script>

<div class="copyright">{#
#}{% set current = date(Date.now(), "YYYY") %}{#
#}&copy; {% if theme.footer.since and theme.footer.since != current %}{{ theme.footer.since }} &mdash; {% endif %}{#
#}<span itemprop="copyrightYear">{{ current }}</span>
  <span class="with-love">
    <i class="fa fa-heart"></i>
  </span>
  <span class="author" itemprop="copyrightHolder">{{ theme.footer.copyright || config.author }}</span>
</div>

{% if theme.footer.powered %}
  <div class="powered-by">{##}
    <i class="fa fa-user-md"></i><span id="busuanzi_container_site_uv">
    访客数  <span id="busuanzi_value_site_uv"></span></span>
  <span class="post-meta-divider">|</span>
  </div>

  <div class="powered-by">
    <i class="fa fa-bolt"></i><span id="busuanzi_container_site_pv">
    点击量  <span id="busuanzi_value_site_pv"></span></span>
  <span class="post-meta-divider">|</span>
  </div>

  <div class="powered-by">
    {% if theme.post_wordcount.totalcount %}
      <span class="post-meta-item-icon">
      <i class="fa fa-pencil"></i></span>
      博客全站共
      <span title="{{ __('post.totalcount') }}">{#
      #}{{ totalcount(site, '0,0.0a') }}{#
      #} 字</span>
    {% endif %}
   </div>
{% endif %}

{% if theme.footer.powered and theme.footer.theme.enable %}
{% endif %}

{% if theme.footer.theme.enable %}
{% endif %}

{% if theme.footer.custom_text %}
  <div class="footer-custom">{#
  #}{{ theme.footer.custom_text }}{#
#}</div>
{% endif %}
